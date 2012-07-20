#include "include/AnnoGraphicsBoundingBox.h"
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

        AnnoGraphicsBoundingBox::AnnoGraphicsBoundingBox(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsRectItem(parent), AnnoGraphicsShape(anno) {
            setupAppearance();
            setRect(*annoBoundingBox());
            initControlPoints();
        }

        AnnoGraphicsBoundingBox::~AnnoGraphicsBoundingBox() {
        }

        QRectF AnnoGraphicsBoundingBox::mapRectToParent(const QRectF &r) const {
            QPointF p0 = mapToParent(r.topLeft());
            QSizeF s = r.size();

            return QRectF(p0, s);
        }

        void AnnoGraphicsBoundingBox::initControlPoints() {
            insertControlPoint(0, new AnnoGraphicsControlPoint(this, 0));
            insertControlPoint(1, new AnnoGraphicsControlPoint(this, 1));
            insertControlPoint(2, new AnnoGraphicsControlPoint(this, 2));
            insertControlPoint(3, new AnnoGraphicsControlPoint(this, 3));
            setControlPointsVisible(false);
            validateCpPos();
        }

        void AnnoGraphicsBoundingBox::validateCpPos() {
            QRectF rect = *annoBoundingBox();
            QPointF p = rect.topLeft();
            moveControlPointTo(0, p.x(), p.y());
            p = rect.bottomLeft();
            moveControlPointTo(1, p.x(), p.y());
            p = rect.bottomRight();
            moveControlPointTo(2, p.x(), p.y());
            p = rect.topRight();
            moveControlPointTo(3, p.x(), p.y());
        }

        QGraphicsItem *AnnoGraphicsBoundingBox::graphicsItem() {
            return this;
        }

        void AnnoGraphicsBoundingBox::shapeMoveBy(qreal deltaX, qreal deltaY) {
            QPointF delta(deltaX, deltaY);
            QRectF tmpRect = mapRectToParent(*annoBoundingBox());
            QRectF parRect = parentItem()->boundingRect();
            tmpRect.moveTo(tmpRect.topLeft() + delta);

            if (parRect.contains(tmpRect)) {
                prepareGeometryChange();
                *annoBoundingBox() = tmpRect;
                setRect(tmpRect);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsBoundingBox::shapeSizeBy(qreal facX, qreal facY) {
            //TODO must be implemented!
        }

        dt::AnnoShapeType AnnoGraphicsBoundingBox::shapeType() const {
            return dt::ASTypeBoundingBox;
        }

        void AnnoGraphicsBoundingBox::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent) {
            dt::Annotation *anno = relatedAnno();
            if(GlobalProjectManager::instance()->isAnnoSelected(anno)) {
                GlobalToolManager::instance()->triggerShapeContextMenu(relatedAnno());
            }
        }

        void AnnoGraphicsBoundingBox::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mousePressEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mouseReleaseEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mouseDoubleClickEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseDoubleClickEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: hoverEnterEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverEnterEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: hoverLeaveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverLeaveEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: hoverMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverMoveEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::keyPressEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: keyPressEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyPressEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::keyReleaseEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: keyReleaseEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyReleaseEvent(this, event);
            }
        }

        QVariant AnnoGraphicsBoundingBox::itemChange(GraphicsItemChange change, const QVariant &value) {
            if(change == QGraphicsItem::ItemSelectedChange) {
                if(value.toBool()) {
                    _bShowMask = true;
                    setControlPointsVisible(true);
                } else {
                    _bShowMask = false;
                    setControlPointsVisible(false);
                }
            }
            return QGraphicsRectItem::itemChange(change, value);
        }

        void AnnoGraphicsBoundingBox::paint(QPainter *painter,
                                            const QStyleOptionGraphicsItem *option, QWidget *widget) {
            dt::AnnoBoundingBox *bBox = annoBoundingBox();
            QImage *qImg = bBox->getImage();
            QRectF imgRect = bBox->boundingRect();
//			imgRect.setX(imgRect.x() - 1.0);
//			imgRect.setY(imgRect.y() - 2.0);
            //QGraphicsView::mapToScene() mapFromScene() ?
            if (NULL != qImg && _bShowMask) {
                painter->drawImage(/*QPoint(0, 0)*/imgRect, *qImg);
            }

            _shapeConfig.applyShapeConfig(painter, isSelected());
            painter->drawRect(rect());

//			/** MA: optionally draw the track-id */
//                        assert(_anno != 0);
//			QString qsTrackId;
//
//			if (_anno->getClassAttributeValue(NATIVE_CLASS_ANNORECT, NATIVE_ANNORECT_ID_ATTR, qsTrackId))		// anna: not sure
//			{
//
//			  /** get the text extent */
//			  QFont font = painter->font();
//			  font.setPointSize(10);
//			  //painter->setFont(QFont("Arial", 7));
//			  painter->setFont(font);
//
//			  QRectF brect = boundingRect();
//			  QRectF textRect(brect.right(), brect.bottom(), 0, 0);
//
//			  QRectF requiredRect = painter->boundingRect(textRect, Qt::AlignLeft | Qt::AlignTop, qsTrackId);
//			  requiredRect.setHeight(0.8*requiredRect.height());
//
//			  int textOffset = 0;
//			  textRect.setLeft(brect.left() + 1);
//			  textRect.setTop(brect.top() + 1);
//			  textRect.setWidth(requiredRect.width() + textOffset);
//			  textRect.setHeight(requiredRect.height() + textOffset);
//
//			  painter->setPen(Qt::red);
//			  painter->drawText(textRect, qsTrackId);
//			}
        }

        void AnnoGraphicsBoundingBox::setupAppearance() {
            setFlag(QGraphicsItem::ItemIsSelectable);
            setFlag(QGraphicsItem::ItemIsFocusable);
            setAcceptsHoverEvents(true);
            setVisible(true);
            setToolTip(_anno->annoInfo());
        }

        dt::AnnoBoundingBox *AnnoGraphicsBoundingBox::annoBoundingBox() {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeBoundingBox) {
                return dynamic_cast<dt::AnnoBoundingBox *>(_anno->shape());
            }
            return NULL;
        }

        const dt::AnnoBoundingBox *AnnoGraphicsBoundingBox::annoBoundingBox() const {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeBoundingBox) {
                return dynamic_cast<dt::AnnoBoundingBox *>(_anno->shape());
            }
            return NULL;
        }

        void AnnoGraphicsBoundingBox::cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_BOUNDING_BOX: cpMousePressEvent on CP %1").arg(index));
        }

        void AnnoGraphicsBoundingBox::cpMouseReleaseEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_BOUNDING_BOX: cpMouseReleaseEvent on CP %1").arg(index));
            prepareGeometryChange();
            *annoBoundingBox() = annoBoundingBox()->normalized();
            setRect(*annoBoundingBox());
            validateCpPos();
            _anno->setModified(true);
            setToolTip(_anno->annoInfo());
        }

        void AnnoGraphicsBoundingBox::cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_BOUNDING_BOX: cpMouseMoveEvent on CP %1").arg(index));
            qreal deltaX = event->scenePos().x() - event->lastScenePos().x();
            qreal deltaY = event->scenePos().y() - event->lastScenePos().y();
            QPointF delta(deltaX, deltaY);

            QRectF rect = *annoBoundingBox();
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
                *annoBoundingBox() = tmpRect;
                setRect(tmpRect);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsBoundingBox::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_BOUNDING_BOX: exMouseMoveEvent.");
            mouseMoveEvent(event);
        }

        void AnnoGraphicsBoundingBox::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_BOUNDING_BOX: exMousePressEvent.");
            mousePressEvent(event);
        }

        void AnnoGraphicsBoundingBox::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_BOUNDING_BOX: exMouseReleaseEvent.");
            mouseReleaseEvent(event);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
