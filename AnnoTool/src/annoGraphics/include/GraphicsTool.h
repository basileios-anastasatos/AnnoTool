#ifndef GRAPHICSTOOL_H_
#define GRAPHICSTOOL_H_

class QGraphicsSceneMouseEvent;

namespace anno {
    namespace graphics {
        class AnnoGraphicsControlPoint;
        class AnnoGraphicsScene;
        class AnnoGraphicsShape;
        class AnnoGraphicsPixmap;

        class GraphicsTool {
                // Members that are managed for child classes
            protected:
                AnnoGraphicsScene *_scene;

                // constructors
            protected:
                GraphicsTool(AnnoGraphicsScene *scene);

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

        };

    }
}

#endif /*GRAPHICSTOOL_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
