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

namespace anno {
    namespace graphics {

        AnnoGraphicsEllipse::AnnoGraphicsEllipse(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsItem(parent), AnnoGraphicsShape(anno) {
            setupAppearance();
        }

        AnnoGraphicsEllipse::AnnoGraphicsEllipse(dt::Annotation *anno, const QRectF &rect,
                QGraphicsItem *parent) :
            QGraphicsItem(parent), AnnoGraphicsShape(anno) {
            *annoEllipse() = rect;
            setupAppearance();
        }

        AnnoGraphicsEllipse::~AnnoGraphicsEllipse() {
        }

        void AnnoGraphicsEllipse::setupAppearance() {
            initControlPoints();
            setFlag(QGraphicsItem::ItemIsSelectable);
            setVisible(true);
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
            *annoEllipse() = mapRectToParent(rect);
            validateCpPos();
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
                validateCpPos();
                _anno->setModified(true);
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
                _anno->setModified(true);
                validateCpPos();

                //setToolTip(QString("%1\n(%2, %3) (%4, %5)").arg(relatedAnno()->annoIdAsString()).arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height()));
            }
        }

        void AnnoGraphicsEllipse::shapeSizeBy(qreal facX, qreal facY) {
            //TODO implement this!
        }

        QRectF AnnoGraphicsEllipse::boundingRect() const {
            const dt::AnnoEllipse *ae = annoEllipse();
            if(ae == NULL) {
                GlobalLogger::instance()->logError("AnnoGraphicsEllipse::boundingRect(): ellipse is NULL");
                GlobalLogger::instance()->logError(QString("Shape-Type: %1").arg(_anno->shape()->shapeType()));

            }
            QRectF b(ae->x() - 1, ae->y() - 1, ae->width() + 2, ae->height() + 2);
            return b;
        }

        void AnnoGraphicsEllipse::paint(QPainter *painter,
                                        const QStyleOptionGraphicsItem *option, QWidget *widget) {
            //TODO nice painting
            QRectF el = *annoEllipse();
            GlobalLogger::instance()->logDebug(QString("AG_ELLIPSE: paint (%1,%2, %3,%4)").arg(el.x()).arg(el.y()).arg(el.width()).arg(el.height()));
            if (isSelected()) {
                setControlPointsVisible(true);
                QPen penNormal(QColor(30, 30, 255, 255));
                penNormal.setWidth(2);
                QBrush brushColor(QColor(255, 255, 0, 200));
                painter->setBrush(brushColor);
                painter->setPen(penNormal);
                painter->drawEllipse(el);
            } else {
                setControlPointsVisible(false);
                QPen penNormal(QColor(30, 30, 255, 255));
                penNormal.setWidth(1);
                QBrush brushColor(QColor(255, 255, 0, 128));
                painter->setBrush(brushColor);
                painter->setPen(penNormal);
                painter->drawEllipse(el);
            }
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
