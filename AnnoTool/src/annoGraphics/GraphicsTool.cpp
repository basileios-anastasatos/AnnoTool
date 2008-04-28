#include "include/GraphicsTool.h"
#include "AllAnnoGraphics.h"
#include <QGraphicsSceneMouseEvent>

namespace anno {
    namespace graphics {

        GraphicsTool::GraphicsTool(AnnoGraphicsScene *scene) {
            _scene = scene;
        }

        GraphicsTool::~GraphicsTool() {
        }

        void GraphicsTool::mousePressEvent(AnnoGraphicsControlPoint *cp,
                                           QGraphicsSceneMouseEvent *event) {
            // absorb event!
            event->accept();
        }

        void GraphicsTool::mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                             QGraphicsSceneMouseEvent *event) {
            // absorb event!
            event->accept();
        }

        void GraphicsTool::mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneMouseEvent *event) {
            // absorb event!
            event->accept();
        }

        void GraphicsTool::mousePressEvent(AnnoGraphicsShape *shape,
                                           QGraphicsSceneMouseEvent *event) {
            // absorb event!
            event->accept();
        }

        void GraphicsTool::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneMouseEvent *event) {
            // absorb event!
            event->accept();
        }

        void GraphicsTool::mouseMoveEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            // absorb event!
            event->accept();
        }

        void GraphicsTool::mousePressEvent(AnnoGraphicsPixmap *img,
                                           QGraphicsSceneMouseEvent *event) {
            // absorb event!
            event->accept();
        }

        void GraphicsTool::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneMouseEvent *event) {
            // absorb event!
            event->accept();
        }

        void GraphicsTool::mouseMoveEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneMouseEvent *event) {
            // absorb event!
            event->accept();
        }

    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
