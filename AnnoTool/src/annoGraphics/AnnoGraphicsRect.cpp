#include "include/AnnoGraphicsRect.h"
#include <QMap>
#include <QRectF>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include "importGlobals.h"

#include "AnnoAvClassList.h"

#include <iostream>

namespace anno {
    namespace graphics {

        AnnoGraphicsRect::AnnoGraphicsRect(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsRectItem(parent), AnnoGraphicsShape(anno) {
            setupAppearance();
            setRect(*annoRect());
            initControlPoints();
        }

        AnnoGraphicsRect::~AnnoGraphicsRect() {
        }

        QRectF AnnoGraphicsRect::mapRectToParent(const QRectF &r) const {
            QPointF p0 = mapToParent(r.topLeft());
            QSizeF s = r.size();

            return QRectF(p0, s);
        }

        void AnnoGraphicsRect::initControlPoints() {
            insertControlPoint(0, new AnnoGraphicsControlPoint(this, 0));
            insertControlPoint(1, new AnnoGraphicsControlPoint(this, 1));
            insertControlPoint(2, new AnnoGraphicsControlPoint(this, 2));
            insertControlPoint(3, new AnnoGraphicsControlPoint(this, 3));
            setControlPointsVisible(false);
            validateCpPos();
        }

        void AnnoGraphicsRect::validateCpPos() {
            QRectF rect = *annoRect();
            QPointF p = rect.topLeft();
            moveControlPointTo(0, p.x(), p.y());
            p = rect.bottomLeft();
            moveControlPointTo(1, p.x(), p.y());
            p = rect.bottomRight();
            moveControlPointTo(2, p.x(), p.y());
            p = rect.topRight();
            moveControlPointTo(3, p.x(), p.y());
        }

        QGraphicsItem *AnnoGraphicsRect::graphicsItem() {
            return this;
        }

        void AnnoGraphicsRect::shapeMoveBy(qreal deltaX, qreal deltaY) {
            QPointF delta(deltaX, deltaY);
            QRectF tmpRect = mapRectToParent(*annoRect());
            QRectF parRect = parentItem()->boundingRect();
            tmpRect.moveTo(tmpRect.topLeft() + delta);

            if (parRect.contains(tmpRect)) {
                prepareGeometryChange();
                *annoRect() = tmpRect;
                setRect(tmpRect);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsRect::shapeSizeBy(qreal facX, qreal facY) {
            //TODO must be implemented!
        }

        dt::AnnoShapeType AnnoGraphicsRect::shapeType() const {
            return dt::ASTypeRectangle;
        }

        void AnnoGraphicsRect::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent) {
            dt::Annotation *anno = relatedAnno();
            if(GlobalProjectManager::instance()->isAnnoSelected(anno)) {
                GlobalToolManager::instance()->triggerShapeContextMenu(relatedAnno());
            }
        }

        void AnnoGraphicsRect::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mousePressEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mouseReleaseEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsRect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mouseDoubleClickEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseDoubleClickEvent(this, event);
            }
        }

        void AnnoGraphicsRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: hoverEnterEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverEnterEvent(this, event);
            }
        }

        void AnnoGraphicsRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: hoverLeaveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverLeaveEvent(this, event);
            }
        }

        void AnnoGraphicsRect::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: hoverMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverMoveEvent(this, event);
            }
        }

        void AnnoGraphicsRect::keyPressEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: keyPressEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyPressEvent(this, event);
            }
        }

        void AnnoGraphicsRect::keyReleaseEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: keyReleaseEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyReleaseEvent(this, event);
            }
        }

        QVariant AnnoGraphicsRect::itemChange(GraphicsItemChange change, const QVariant &value) {
            if(change == QGraphicsItem::ItemSelectedChange) {
                if(value.toBool()) {
                    setControlPointsVisible(true);
                } else {
                    setControlPointsVisible(false);
                }
            }
            return QGraphicsRectItem::itemChange(change, value);
        }

        void AnnoGraphicsRect::paint(QPainter *painter,
                                     const QStyleOptionGraphicsItem *option, QWidget *widget) {
            _shapeConfig.applyShapeConfig(painter, isSelected());
            painter->drawRect(rect());

            /** MA: optionally draw the track-id */
            assert(_anno != 0);
            QString qsTrackId;

            if (_anno->getClassAttributeValue(NATIVE_CLASS_ANNORECT, NATIVE_ANNORECT_ID_ATTR, qsTrackId)) {

                /** get the text extent */
                QFont font = painter->font();
                font.setPointSize(10);
                //painter->setFont(QFont("Arial", 7));
                painter->setFont(font);

                QRectF brect = boundingRect();
                QRectF textRect(brect.right(), brect.bottom(), 0, 0);

                QRectF requiredRect = painter->boundingRect(textRect, Qt::AlignLeft | Qt::AlignTop, qsTrackId);
                requiredRect.setHeight(0.8 * requiredRect.height());

                int textOffset = 0;
                textRect.setLeft(brect.left() + 1);
                textRect.setTop(brect.top() + 1);
                textRect.setWidth(requiredRect.width() + textOffset);
                textRect.setHeight(requiredRect.height() + textOffset);

                painter->setPen(Qt::red);
                painter->drawText(textRect, qsTrackId);
            }
        }

        void AnnoGraphicsRect::setupAppearance() {
            setFlag(QGraphicsItem::ItemIsSelectable);
            setFlag(QGraphicsItem::ItemIsFocusable);
            setAcceptsHoverEvents(true);
            setVisible(true);
            setToolTip(_anno->annoInfo());
        }

        dt::AnnoRectangle *AnnoGraphicsRect::annoRect() {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeRectangle) {
                return reinterpret_cast<dt::AnnoRectangle *>(_anno->shape());
            }
            return NULL;
        }

        const dt::AnnoRectangle *AnnoGraphicsRect::annoRect() const {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeRectangle) {
                return reinterpret_cast<dt::AnnoRectangle *>(_anno->shape());
            }
            return NULL;
        }

        void AnnoGraphicsRect::cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_RECT: cpMousePressEvent on CP %1").arg(index));
        }

        void AnnoGraphicsRect::cpMouseReleaseEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_RECT: cpMouseReleaseEvent on CP %1").arg(index));
            prepareGeometryChange();
            *annoRect() = annoRect()->normalized();
            setRect(*annoRect());
            validateCpPos();
            _anno->setModified(true);
            setToolTip(_anno->annoInfo());
        }

        void AnnoGraphicsRect::cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_RECT: cpMouseMoveEvent on CP %1").arg(index));
            qreal deltaX = event->scenePos().x() - event->lastScenePos().x();
            qreal deltaY = event->scenePos().y() - event->lastScenePos().y();
            QPointF delta(deltaX, deltaY);

            QRectF rect = *annoRect();
            QPointF nPoint;
            switch (index) {
                case 0:
                    nPoint = rect.topLeft();
                    nPoint += delta;
                    rect.setTopLeft(nPoint);
                    break;
                case 1:
                    nPoint = rect.bottomLeft();
                    nPoint += delta;
                    rect.setBottomLeft(nPoint);
                    break;
                case 2:
                    nPoint = rect.bottomRight();
                    nPoint += delta;
                    rect.setBottomRight(nPoint);
                    break;
                case 3:
                    nPoint = rect.topRight();
                    nPoint += delta;
                    rect.setTopRight(nPoint);
                    break;
            }
            QRectF tmpRect = mapRectToParent(rect);
            QRectF parRect = parentItem()->boundingRect();
            if (parRect.contains(tmpRect)) {
                prepareGeometryChange();
                *annoRect() = tmpRect;
                setRect(tmpRect);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsRect::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: exMouseMoveEvent.");
            mouseMoveEvent(event);
        }

        void AnnoGraphicsRect::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: exMousePressEvent.");
            mousePressEvent(event);
        }

        void AnnoGraphicsRect::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: exMouseReleaseEvent.");
            mouseReleaseEvent(event);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
