#ifndef TOOLELLIPSE_H_
#define TOOLELLIPSE_H_

#include "GraphicsTool.h"

namespace anno {
    namespace graphics {
        class AnnoGraphicsShape;

        class ToolEllipse : public anno::graphics::GraphicsTool {
            private:
                QCursor _cursorNormal;
                QCursor _cursorActive;
                AnnoGraphicsShape *_curShape;

            public:
                ToolEllipse(QGraphicsView *view, AnnoGraphicsScene *scene);
                virtual ~ToolEllipse();

                // general tool information
            public:
                virtual bool handlesCp() const;
                virtual bool handlesShape() const;
                virtual bool handlesImage() const;

                // mouse interface
            public:
                virtual void mousePressEvent(AnnoGraphicsControlPoint *cp,
                                             QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                               QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                            QGraphicsSceneMouseEvent *event);

                virtual void mousePressEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(AnnoGraphicsShape *shape,
                                               QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(AnnoGraphicsShape *shape,
                                            QGraphicsSceneMouseEvent *event);

                virtual void mousePressEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                               QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(AnnoGraphicsPixmap *img,
                                            QGraphicsSceneMouseEvent *event);

                virtual void hoverEnterEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneHoverEvent *event);
        };

    }
}

#endif /*TOOLELLIPSE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
