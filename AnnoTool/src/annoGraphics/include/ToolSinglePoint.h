#ifndef TOOLSINGLEPOINT_H_
#define TOOLSINGLEPOINT_H_

#include "GraphicsTool.h"

namespace anno {
    namespace graphics {
        class AnnoGraphicsShape;

        class ToolSinglePoint : public anno::graphics::GraphicsTool {
            private:
                QCursor _cursorNormal;
                QCursor _cursorActive;

            private:
                void switchDefaultTool();

            public:
                ToolSinglePoint(QGraphicsView *view, AnnoGraphicsScene *scene);
                virtual ~ToolSinglePoint();

                // general tool information
            public:
                virtual bool handlesCp() const;
                virtual bool handlesShape() const;
                virtual bool handlesImage() const;

                // general tool interface
            public:
                //virtual void toolActivate();
                //virtual void toolDeactivate();

                // mouse interface
            public:

                virtual void mousePressEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneMouseEvent *event);
//				virtual void mouseReleaseEvent(AnnoGraphicsShape* shape,
//						QGraphicsSceneMouseEvent* event);
//				virtual void mouseMoveEvent(AnnoGraphicsShape* shape,
//						QGraphicsSceneMouseEvent* event);

                virtual void mousePressEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                               QGraphicsSceneMouseEvent *event);
//				virtual void mouseMoveEvent(AnnoGraphicsPixmap* img,
//						QGraphicsSceneMouseEvent* event);

                virtual void hoverEnterEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneHoverEvent *event);

        };

    }
}

#endif /*TOOLSINGLEPOINT_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
