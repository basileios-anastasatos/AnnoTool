#include "include/GraphicsTool.h"
#include "AllAnnoGraphics.h"
#include <QGraphicsView>

namespace anno {
    namespace graphics {

        GraphicsTool::GraphicsTool(QGraphicsView *view, AnnoGraphicsScene *scene) :
            _view(view), _scene(scene) {
        }

        GraphicsTool::~GraphicsTool() {
        }

        void GraphicsTool::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mousePressEvent(AnnoGraphicsControlPoint *cp,
                                           QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                             QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mouseDoubleClickEvent(AnnoGraphicsControlPoint *cp,
                QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mousePressEvent(AnnoGraphicsShape *shape,
                                           QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mouseDoubleClickEvent(AnnoGraphicsShape *shape,
                QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mouseMoveEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mousePressEvent(AnnoGraphicsPixmap *img,
                                           QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mouseDoubleClickEvent(AnnoGraphicsPixmap *img,
                QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::mouseMoveEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneMouseEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::hoverEnterEvent(AnnoGraphicsControlPoint *cp,
                                           QGraphicsSceneHoverEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::hoverLeaveEvent(AnnoGraphicsControlPoint *cp,
                                           QGraphicsSceneHoverEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::hoverMoveEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneHoverEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::hoverEnterEvent(AnnoGraphicsShape *shape,
                                           QGraphicsSceneHoverEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::hoverLeaveEvent(AnnoGraphicsShape *shape,
                                           QGraphicsSceneHoverEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::hoverMoveEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneHoverEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::hoverEnterEvent(AnnoGraphicsPixmap *img,
                                           QGraphicsSceneHoverEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::hoverLeaveEvent(AnnoGraphicsPixmap *img,
                                           QGraphicsSceneHoverEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::hoverMoveEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneHoverEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::keyPressEvent(AnnoGraphicsControlPoint *cp, QKeyEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::keyReleaseEvent(AnnoGraphicsControlPoint *cp, QKeyEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::keyPressEvent(AnnoGraphicsShape *shape, QKeyEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::keyReleaseEvent(AnnoGraphicsShape *shape, QKeyEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::keyPressEvent(AnnoGraphicsPixmap *img, QKeyEvent *event) {
            // Do nothing as default!
        }

        void GraphicsTool::keyReleaseEvent(AnnoGraphicsPixmap *img, QKeyEvent *event) {
            // Do nothing as default!
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
