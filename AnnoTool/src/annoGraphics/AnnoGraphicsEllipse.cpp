#include "include/AnnoGraphicsEllipse.h"
#include <QRectF>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include "importGlobals.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>

namespace anno {
    namespace graphics {

        AnnoGraphicsEllipse::AnnoGraphicsEllipse(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsEllipseItem(parent), AnnoGraphicsShape(anno) {
            setupAppearance();
            setRect(*annoEllipse());
            initControlPoints();
        }

        AnnoGraphicsEllipse::~AnnoGraphicsEllipse() {
        }

        void AnnoGraphicsEllipse::setupAppearance() {
            ShapeConfig sc = GlobalConfig::instance()->getShapeConfig("ellipse");
            setPen(sc.penNormal);
            setBrush(sc.brushNormal);
            setFlag(QGraphicsItem::ItemIsSelectable);
            setVisible(true);
            setAcceptsHoverEvents(true);
            setToolTip(_anno->annoInfo());
        }

        dt::AnnoEllipse *AnnoGraphicsEllipse::annoEllipse() {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeEllipse) {
                return reinterpret_cast<dt::AnnoEllipse *>(_anno->shape());
            }
            return NULL;
        }

        const dt::AnnoEllipse *AnnoGraphicsEllipse::annoEllipse() const {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeEllipse) {
                return reinterpret_cast<dt::AnnoEllipse *>(_anno->shape());
            }
            return NULL;
        }

        QRectF AnnoGraphicsEllipse::mapRectToParent(const QRectF &r) const {
            QPointF p0 = mapToParent(r.topLeft());
            QSizeF s = r.size();

            return QRectF(p0, s);
        }

        void AnnoGraphicsEllipse::initControlPoints() {
            insertControlPoint(0, new AnnoGraphicsControlPoint(this, 0));
            insertControlPoint(1, new AnnoGraphicsControlPoint(this, 1));
            insertControlPoint(2, new AnnoGraphicsControlPoint(this, 2));
            insertControlPoint(3, new AnnoGraphicsControlPoint(this, 3));
            setControlPointsVisible(false);
            validateCpPos();
        }

        void AnnoGraphicsEllipse::validateCpPos() {
            QRectF ae = *annoEllipse();
            QPointF p = ae.topLeft();
            moveControlPointTo(0, p.x(), p.y());
            p = ae.bottomLeft();
            moveControlPointTo(1, p.x(), p.y());
            p = ae.bottomRight();
            moveControlPointTo(2, p.x(), p.y());
            p = ae.topRight();
            moveControlPointTo(3, p.x(), p.y());
        }

        void AnnoGraphicsEllipse::cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_ELLIPSE: cpMousePressEvent on CP %1").arg(index));
        }

        void AnnoGraphicsEllipse::cpMouseReleaseEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_ELLIPSE: cpMouseReleaseEvent on CP %1").arg(index));
            QRectF rect = annoEllipse()->normalized();
            prepareGeometryChange();
            *annoEllipse() = rect;//mapRectToParent(rect);
            setRect(rect);
            validateCpPos();
            _anno->setModified(true);
            setToolTip(_anno->annoInfo());
        }

        void AnnoGraphicsEllipse::cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_Ellipse: cpMouseMoveEvent on CP %1").arg(index));
            qreal deltaX = event->scenePos().x() - event->lastScenePos().x();
            qreal deltaY = event->scenePos().y() - event->lastScenePos().y();
            QPointF delta(deltaX, deltaY);

            QRectF rect = *annoEllipse();
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
                *annoEllipse() = tmpRect;
                setRect(tmpRect);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsEllipse::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: mousePressEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsEllipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: mouseReleaseEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsEllipse::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsEllipse::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: hoverEnterEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverEnterEvent(this, event);
            }
        }

        void AnnoGraphicsEllipse::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: hoverLeaveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverLeaveEvent(this, event);
            }
        }

        void AnnoGraphicsEllipse::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: hoverMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverMoveEvent(this, event);
            }
        }

        void AnnoGraphicsEllipse::keyPressEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: keyPressEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyPressEvent(this, event);
            }
        }

        void AnnoGraphicsEllipse::keyReleaseEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: keyReleaseEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyReleaseEvent(this, event);
            }
        }

        QVariant AnnoGraphicsEllipse::itemChange(GraphicsItemChange change,
                const QVariant &value) {
            if (change == QGraphicsItem::ItemSelectedChange) {
                ShapeConfig sc = GlobalConfig::instance()->getShapeConfig("ellipse");
                if (value.toBool()) {
                    setControlPointsVisible(true);
                    setPen(sc.penSelected);
                    setBrush(sc.brushSelected);
                } else {
                    setControlPointsVisible(false);
                    setPen(sc.penNormal);
                    setBrush(sc.brushNormal);
                }
            }
            return QGraphicsEllipseItem::itemChange(change, value);
        }

        QGraphicsItem *AnnoGraphicsEllipse::graphicsItem() {
            return this;
        }

        void AnnoGraphicsEllipse::shapeMoveBy(qreal deltaX, qreal deltaY) {
            QPointF delta(deltaX, deltaY);
            dt::AnnoEllipse *ae = annoEllipse();
            QRectF tmpRect = mapRectToParent(*ae);
            QRectF parRect = parentItem()->boundingRect();
            tmpRect.moveTo(tmpRect.topLeft() + delta);

            if (parRect.contains(tmpRect)) {
                prepareGeometryChange();
                *ae = tmpRect;
                setRect(tmpRect);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsEllipse::shapeSizeBy(qreal facX, qreal facY) {
            //TODO implement this!
        }

        dt::AnnoShapeType AnnoGraphicsEllipse::shapeType() const {
            return dt::ASTypeEllipse;
        }

        void AnnoGraphicsEllipse::paint(QPainter *painter,
                                        const QStyleOptionGraphicsItem *option, QWidget *widget) {
            painter->setPen(pen());
            painter->setBrush(brush());
            painter->drawEllipse(rect());
        }

        void AnnoGraphicsEllipse::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: exMouseMoveEvent.");
            mouseMoveEvent(event);
        }

        void AnnoGraphicsEllipse::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: exMousePressEvent.");
            mousePressEvent(event);
        }

        void AnnoGraphicsEllipse::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_ELLIPSE: exMouseReleaseEvent.");
            mouseReleaseEvent(event);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
