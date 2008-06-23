#include "include/AnnoGraphicsRect.h"
#include <QMap>
#include <QRectF>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include "importGlobals.h"

namespace anno {
    namespace graphics {

        AnnoGraphicsRect::AnnoGraphicsRect(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsRectItem(parent), AnnoGraphicsShape(anno) {
            QRectF r = QGraphicsRectItem::rect();
            setupAppearance();
        }

        AnnoGraphicsRect::AnnoGraphicsRect(dt::Annotation *anno, const QRectF &rect,
                                           QGraphicsItem *parent) :
            QGraphicsRectItem(rect, parent), AnnoGraphicsShape(anno) {
            initControlPoints();
            setupAppearance();
        }

        AnnoGraphicsRect::AnnoGraphicsRect(dt::Annotation *anno, qreal x, qreal y,
                                           qreal width, qreal height, QGraphicsItem *parent) :
            QGraphicsRectItem(x, y, width, height, parent), AnnoGraphicsShape(anno) {
            initControlPoints();
            setupAppearance();
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
            QRectF rect = QGraphicsRectItem::rect();
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
            QRectF tmpRect = mapRectToParent(rect());
            QRectF parRect = parentItem()->boundingRect();
            tmpRect.moveTo(tmpRect.topLeft() + delta);

            if (parRect.contains(tmpRect)) {
                prepareGeometryChange();
                moveBy(deltaX, deltaY);
                QRectF rect = mapRectToParent(QGraphicsRectItem::rect());
                *annoRect() = rect;
                _anno->setModified(true);
                setToolTip(QString("%1\n%2").arg(_anno->annoIdAsString()).arg(_anno->shape()->shapeInfo()));
            }
        }

        void AnnoGraphicsRect::shapeSizeBy(qreal facX, qreal facY) {
            //TODO must be implemented!
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

        void AnnoGraphicsRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsRect::paint(QPainter *painter,
                                     const QStyleOptionGraphicsItem *option, QWidget *widget) {
            GlobalLogger::instance()->logDebug("AG_RECT: paint.");
            if (isSelected()) {
                setControlPointsVisible(true);
                painter->setPen(GlobalConfig::shapeColors.penSelected);
                painter->setBrush(GlobalConfig::shapeColors.brushSelected);
            } else {
                setControlPointsVisible(false);
                painter->setPen(GlobalConfig::shapeColors.penNormal);
                painter->setBrush(GlobalConfig::shapeColors.brushNormal);
            }
            painter->drawRect(rect());
        }

        void AnnoGraphicsRect::setupAppearance() {
            QPen pen(QColor(30, 30, 255, 255));
            pen.setWidth(1);
            setPen(pen);
            setBrush(QBrush(QColor(255, 255, 60, 45)));
            setFlag(QGraphicsItem::ItemIsSelectable);
            setVisible(true);
            setToolTip(QString("%1\n%2").arg(_anno->annoIdAsString()).arg(_anno->shape()->shapeInfo()));
        }

        dt::AnnoRectangle *AnnoGraphicsRect::annoRect() {
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
            QRectF rect = QGraphicsRectItem::rect().normalized();
            prepareGeometryChange();
            setRect(rect);
            *annoRect() = mapRectToParent(rect);
            validateCpPos();
            _anno->setModified(true);
            setToolTip(QString("%1\n%2").arg(_anno->annoIdAsString()).arg(_anno->shape()->shapeInfo()));
        }

        void AnnoGraphicsRect::cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_RECT: cpMouseMoveEvent on CP %1").arg(index));
            qreal deltaX = event->scenePos().x() - event->lastScenePos().x();
            qreal deltaY = event->scenePos().y() - event->lastScenePos().y();
            QPointF delta(deltaX, deltaY);

            QRectF rect = QGraphicsRectItem::rect();
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
                setRect(rect);
                *annoRect() = tmpRect;
                annoRect()->print();
                validateCpPos();
                _anno->setModified(true);
                setToolTip(QString("%1\n%2").arg(_anno->annoIdAsString()).arg(_anno->shape()->shapeInfo()));
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
