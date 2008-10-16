#ifndef TOOLRECT_H_
#define TOOLRECT_H_

#include "GraphicsTool.h"

namespace anno {
    namespace dt {
        class Annotation;
    }

    namespace graphics {
        class AnnoGraphicsRect;

        class ToolRect : public GraphicsTool {
            private:
                QCursor _cursorNormal;
                QCursor _cursorActive;
                AnnoGraphicsRect *_curShape;
                dt::Annotation *_curParentAnno;

            private:
                void escape();
                void handleEscape(QKeyEvent *event);
                void switchDefaultTool();

            public:
                ToolRect(QGraphicsView *view, AnnoGraphicsScene *scene);
                virtual ~ToolRect();

                // general tool information
            public:
                virtual bool handlesCp() const;
                virtual bool handlesShape() const;
                virtual bool handlesImage() const;

                // general tool interface
            public:
                //virtual void toolActivate();
                virtual void toolDeactivate();

                // mouse interface
            public:
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

                virtual void keyReleaseEvent(AnnoGraphicsControlPoint *cp, QKeyEvent *event);
                virtual void keyReleaseEvent(AnnoGraphicsShape *shape, QKeyEvent *event);
                virtual void keyReleaseEvent(AnnoGraphicsPixmap *img, QKeyEvent *event);
        };

    }
}

#endif /*TOOLRECT_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
