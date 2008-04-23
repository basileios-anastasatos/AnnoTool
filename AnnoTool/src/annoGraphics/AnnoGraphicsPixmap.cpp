#include "include/AnnoGraphicsPixmap.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>

namespace anno {
    namespace graphics {

        AnnoGraphicsPixmap::AnnoGraphicsPixmap(const QPixmap &pixmap,
                                               QGraphicsItem *parent) :
            QGraphicsPixmapItem(pixmap, parent) {
        }

        AnnoGraphicsPixmap::~AnnoGraphicsPixmap() {
        }

        void AnnoGraphicsPixmap::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            QGraphicsPixmapItem::mouseMoveEvent(event);
        }

        void AnnoGraphicsPixmap::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            QGraphicsPixmapItem::mousePressEvent(event);
        }

        void AnnoGraphicsPixmap::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            QGraphicsPixmapItem::mouseReleaseEvent(event);
        }

//		void AnnoGraphicsPixmap::mousePressEvent(QGraphicsSceneMouseEvent* event)
//		{
//			if (event->button() == Qt::LeftButton && (event->modifiers()
//					& Qt::ShiftModifier) != 0 && (event->modifiers()
//					& Qt::ControlModifier) != 0)
//			{
//								AnnoGraphicsPolygon* poly = new AnnoGraphicsPolygon();
//								QPolygonF p;
//								p << event->pos();
//								poly->setPolygon(p);
//								poly->setZValue(20);
//								poly->setParentItem(this);
//								poly->initControlPoints();
//								_polygons <<poly;
//			}
//		}

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
