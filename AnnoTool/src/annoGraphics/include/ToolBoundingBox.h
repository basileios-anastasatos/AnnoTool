#pragma once


#include "GraphicsTool.h"

namespace anno {
    namespace dt {
        class Annotation;
    }

    namespace graphics {
        class AnnoGraphicsBoundingBox;

        class ToolBoundingBox : public GraphicsTool {
            private:
                QCursor _cursorNormal;
                QCursor _cursorActive;
                AnnoGraphicsBoundingBox *_curShape;
                dt::Annotation *_curParentAnno;

            private:
                void escape();
                void handleEscape(QKeyEvent *event);
                void switchDefaultTool();

            public:
                ToolBoundingBox(QGraphicsView *view, AnnoGraphicsScene *scene);
                virtual ~ToolBoundingBox();

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

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
