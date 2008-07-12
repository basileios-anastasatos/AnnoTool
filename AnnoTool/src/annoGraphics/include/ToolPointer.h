#ifndef TOOLPOINTER_H_
#define TOOLPOINTER_H_

#include "GraphicsTool.h"
#include <QCursor>

namespace anno {
    namespace graphics {

        class ToolPointer : public anno::graphics::GraphicsTool {
            private:
                QCursor _cursorNormal;
                QCursor _cursorHilight;
                QCursor _cursorMoving;
                QCursor _cursorCp;
                QCursor _cursorCpMoving;

            public:
                ToolPointer(QGraphicsView *view, AnnoGraphicsScene *scene);
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

                virtual void mousePressEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneMouseEvent *event);

                virtual void hoverEnterEvent(AnnoGraphicsControlPoint *cp,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsControlPoint *cp,
                                             QGraphicsSceneHoverEvent *event);

                virtual void hoverEnterEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneHoverEvent *event);

                virtual void hoverEnterEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneHoverEvent *event);
        };

    }
}

#endif /*TOOLPOINTER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
