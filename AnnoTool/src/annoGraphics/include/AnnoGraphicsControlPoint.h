#ifndef ANNOGRAPHICSCONTROLPOINT_H_
#define ANNOGRAPHICSCONTROLPOINT_H_

#include <QGraphicsEllipseItem>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;

namespace anno {
    namespace graphics {
        class AnnoGraphicsShape;

        class AnnoGraphicsControlPoint : public QGraphicsEllipseItem {
            private:
                AnnoGraphicsShape *_parentShape;
                int _index;

            protected:
                virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
                virtual void keyPressEvent(QKeyEvent *event);
                virtual void keyReleaseEvent(QKeyEvent *event);

            public:
                AnnoGraphicsControlPoint(AnnoGraphicsShape *parent, int index);
                virtual ~AnnoGraphicsControlPoint();

            public:
                void setupAppearance();
                void setIndex(int index);
                int getIndex();
                AnnoGraphicsShape *parentShape();
        };

    }
}

#endif /*ANNOGRAPHICSCONTROLPOINT_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
