#ifndef TOOLPOINTER_H_
#define TOOLPOINTER_H_

#include "GraphicsTool.h"

namespace anno {
    namespace graphics {

        class ToolPointer : public anno::graphics::GraphicsTool {
            public:
                ToolPointer(AnnoGraphicsScene *scene);
                virtual ~ToolPointer();

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

                virtual void mousePressEvent(AnnoGraphicsPixmap *shape,
                                             QGraphicsSceneMouseEvent *event);
        };

    }
}

#endif /*TOOLPOINTER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
