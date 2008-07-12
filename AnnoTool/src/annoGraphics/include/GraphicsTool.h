#ifndef GRAPHICSTOOL_H_
#define GRAPHICSTOOL_H_

#include <QStack>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QKeyEvent>

class QGraphicsView;

namespace anno {
    namespace graphics {
        class AnnoGraphicsControlPoint;
        class AnnoGraphicsScene;
        class AnnoGraphicsShape;
        class AnnoGraphicsPixmap;

        class GraphicsTool {
                // Members that are managed for child classes
            protected:
                QGraphicsView *_view;
                AnnoGraphicsScene *_scene;
                QStack<QCursor> _prevCursors;

                // constructors
            protected:
                GraphicsTool(QGraphicsView *view, AnnoGraphicsScene *scene);

                // destructor
            public:
                virtual ~GraphicsTool();

                // general tool information
            public:
                virtual bool handlesCp() const = 0;
                virtual bool handlesShape() const = 0;
                virtual bool handlesImage() const = 0;

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

                virtual void hoverEnterEvent(AnnoGraphicsControlPoint *cp,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsControlPoint *cp,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverMoveEvent(AnnoGraphicsControlPoint *cp,
                                            QGraphicsSceneHoverEvent *event);

                virtual void hoverEnterEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverMoveEvent(AnnoGraphicsShape *shape,
                                            QGraphicsSceneHoverEvent *event);

                virtual void hoverEnterEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverMoveEvent(AnnoGraphicsPixmap *img,
                                            QGraphicsSceneHoverEvent *event);

                virtual void keyPressEvent(AnnoGraphicsControlPoint *cp, QKeyEvent *event);
                virtual void keyReleaseEvent(AnnoGraphicsControlPoint *cp, QKeyEvent *event);

                virtual void keyPressEvent(AnnoGraphicsShape *shape, QKeyEvent *event);
                virtual void keyReleaseEvent(AnnoGraphicsShape *shape, QKeyEvent *event);

                virtual void keyPressEvent(AnnoGraphicsPixmap *img, QKeyEvent *event);
                virtual void keyReleaseEvent(AnnoGraphicsPixmap *img, QKeyEvent *event);

        };

    }
}

#endif /*GRAPHICSTOOL_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
