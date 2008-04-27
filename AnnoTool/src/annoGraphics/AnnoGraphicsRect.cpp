#include "include/AnnoGraphicsRect.h"
#include <QMap>
#include <QGraphicsPixmapItem>
#include <QRectF>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QStyle>
#include <QStyleOptionGraphicsItem>

#include "importGlobals.h"

namespace anno {
    namespace graphics {

        AnnoGraphicsRect::AnnoGraphicsRect(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsRectItem(parent), AnnoGraphicsShape(anno) {
            setupAppearance();
        }

        AnnoGraphicsRect::AnnoGraphicsRect(dt::Annotation *anno, const QRectF &rect,
                                           QGraphicsItem *parent) :
            QGraphicsRectItem(rect, parent), AnnoGraphicsShape(anno) {
            setupAppearance();
            initControlPoints();
        }

        AnnoGraphicsRect::AnnoGraphicsRect(dt::Annotation *anno, qreal x, qreal y,
                                           qreal width, qreal height, QGraphicsItem *parent) :
            QGraphicsRectItem(x, y, width, height, parent), AnnoGraphicsShape(anno) {
            setupAppearance();
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

            if (tmpRect.top() >= 0 && tmpRect.left() >= 0 && tmpRect.bottom()
                    <= parRect.height() && tmpRect.right() <= parRect.width()) {
                prepareGeometryChange();
                moveBy(deltaX, deltaY);
                QRectF rect = mapRectToParent(QGraphicsRectItem::rect());
                *annoRect() = rect;
                annoRect()->print();
                GlobalLogger::instance()->logDebug(QString("Moved rect to (%1;%2))").arg(rect.topLeft().x()).arg(rect.topLeft().y()));
            }
        }

        void AnnoGraphicsRect::shapeSizeBy(qreal facX, qreal facY) {
            //TODO must be implemented!
        }

        void AnnoGraphicsRect::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsRect::paint(QPainter *painter,
                                     const QStyleOptionGraphicsItem *option, QWidget *widget) {
            QStyleOptionGraphicsItem option2(*option);
            if (isSelected()) {
                setControlPointsVisible(true);
                setPen(GlobalConfig::shapeColors.penSelected);
                setBrush(GlobalConfig::shapeColors.brushSelected);
                option2.state = option2.state & ~QStyle::State_Selected;
            } else {
                setControlPointsVisible(false);
                setPen(GlobalConfig::shapeColors.penNormal);
                setBrush(GlobalConfig::shapeColors.brushNormal);
            }
            QGraphicsRectItem::paint(painter, &option2, widget);
        }

        void AnnoGraphicsRect::setupAppearance() {
            QPen pen(QColor(30, 30, 255, 255));
            pen.setWidth(1);
            setPen(pen);
            setBrush(QBrush(QColor(255, 255, 60, 45)));
            setFlag(QGraphicsItem::ItemIsSelectable);
            setVisible(true);
        }

        dt::AnnoRectangle *AnnoGraphicsRect::annoRect() {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeRectangle) {
                return reinterpret_cast<dt::AnnoRectangle *>(_anno->shape());
            }
            return NULL;
        }

        void AnnoGraphicsRect::cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AGRect: CP: MousePressEvent on CP %1").arg(index));
        }

        void AnnoGraphicsRect::cpMouseReleaseEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AGRect: CP: MouseReleaseEvent on CP %1").arg(index));
            QRectF rect = QGraphicsRectItem::rect().normalized();
            prepareGeometryChange();
            setRect(rect);
            *annoRect() = mapRectToParent(rect);
            validateCpPos();
        }

        void AnnoGraphicsRect::cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event) {
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
            if (tmpRect.top() >= 0 && tmpRect.left() >= 0 && tmpRect.bottom()
                    <= parRect.height() && tmpRect.right() <= parRect.width()) {
                prepareGeometryChange();
                setRect(rect);
                *annoRect() = tmpRect;
                annoRect()->print();
                validateCpPos();
            }
        }

        void AnnoGraphicsRect::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            mouseMoveEvent(event);
        }

        void AnnoGraphicsRect::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            mousePressEvent(event);
        }

        void AnnoGraphicsRect::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            mouseReleaseEvent(event);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
