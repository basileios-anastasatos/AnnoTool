#include "include/AnnoGraphicsControlPoint.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

namespace anno {
    namespace graphics {

        AnnoGraphicsControlPoint::AnnoGraphicsControlPoint(
            AnnoGraphicsShape *parent, int index) {
            _parentShape = parent;
            _index = index;

            setRect(0, 0, 10, 10);
            setupAppearance();
        }

        AnnoGraphicsControlPoint::~AnnoGraphicsControlPoint() {
        }

        void AnnoGraphicsControlPoint::mousePressEvent(
            QGraphicsSceneMouseEvent *event) {
//			if (event->button() == Qt::LeftButton && (event->modifiers()
//					& Qt::ShiftModifier) != 0)
//			{
//				AnnoGraphicsControlPoint* cp = new AnnoGraphicsControlPoint(_parentPolygon, _parentPolygonPointIndex);
//				cp->setZValue(20);
//				cp->setParentItem(_parentPolygon);
//				_parentPolygon->insertControlPoint(_parentPolygonPointIndex, cp);
//			}
//			else if (event->button() == Qt::RightButton && (event->modifiers()
//					& Qt::ShiftModifier) != 0)
//			{
//				setParentItem(NULL);
//				_parentPolygon->removeControlPoint(_parentPolygonPointIndex);
//			}
        }

        void AnnoGraphicsControlPoint::mouseMoveEvent(
            QGraphicsSceneMouseEvent *event) {
//			qreal deltaX = event->pos().x() - event->lastPos().x();
//			qreal deltaY = event->pos().y() - event->lastPos().y();
//
//			if ((event->modifiers() & Qt::ShiftModifier) != 0)
//			{
//				//_parentPolygon->rotate(deltaX * 3.2);
//			}
//			else
//			{
//				_parentPolygon->moveControlPoint(_parentPolygonPointIndex, deltaX,
//						deltaY);
//			}
        }

        void AnnoGraphicsControlPoint::setupAppearance() {
            setVisible(true);
            QPen pen(QColor(160, 160, 160, 255));
            pen.setWidth(1);
            setPen(pen);
            setBrush(QBrush(QColor(242, 242, 242, 128)));
        }

        void AnnoGraphicsControlPoint::setIndex(int index) {
            _index = index;
        }
        int AnnoGraphicsControlPoint::getIndex() {
            return _index;
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
