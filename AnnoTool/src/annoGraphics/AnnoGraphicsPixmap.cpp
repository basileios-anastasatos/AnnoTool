#include "include/AnnoGraphicsPixmap.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>

#include "importGlobals.h"

namespace anno {
    namespace graphics {

        AnnoGraphicsPixmap::AnnoGraphicsPixmap(const QPixmap &pixmap, QGraphicsItem *parent) :
            QGraphicsPixmapItem(pixmap, parent) {
        }

        AnnoGraphicsPixmap::~AnnoGraphicsPixmap() {
        }

        void AnnoGraphicsPixmap::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("PIXMAP: mousePressEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            mouseMoveEvent(event);
        }

        void AnnoGraphicsPixmap::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("PIXMAP: external mousePressEvent");
            mousePressEvent(event);
        }

        void AnnoGraphicsPixmap::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            mouseReleaseEvent(event);
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
