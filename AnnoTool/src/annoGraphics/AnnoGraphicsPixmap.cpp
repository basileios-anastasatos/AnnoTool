#include "include/AnnoGraphicsPixmap.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>
#include <QGraphicsSceneHoverEvent>

#include "importGlobals.h"

namespace anno {
    namespace graphics {

        AnnoGraphicsPixmap::AnnoGraphicsPixmap(const QPixmap &pixmap, QGraphicsItem *parent) :
            QGraphicsPixmapItem(pixmap, parent) {
            setAcceptsHoverEvents(true);
        }

        AnnoGraphicsPixmap::~AnnoGraphicsPixmap() {
        }

        void AnnoGraphicsPixmap::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: mousePressEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: mouseReleaseEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: hoverEnterEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverEnterEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: hoverLeaveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverLeaveEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: hoverMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverMoveEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::keyPressEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: keyPressEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyPressEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::keyReleaseEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: keyReleaseEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsPixmap::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: exMouseMoveEvent.");
            mouseMoveEvent(event);
        }

        void AnnoGraphicsPixmap::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: exMousePressEvent.");
            mousePressEvent(event);
        }

        void AnnoGraphicsPixmap::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_PIXMAP: exMouseReleaseEvent.");
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
