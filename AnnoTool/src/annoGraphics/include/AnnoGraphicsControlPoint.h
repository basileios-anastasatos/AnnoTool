#ifndef ANNOGRAPHICSCONTROLPOINT_H_
#define ANNOGRAPHICSCONTROLPOINT_H_

#include <QGraphicsEllipseItem>

class AnnoGraphicsPolygon;
class QGraphicsSceneMouseEvent;

namespace anno {
    namespace graphics {
        class AnnoGraphicsShape;

        class AnnoGraphicsControlPoint : public QGraphicsEllipseItem {
            private:
                AnnoGraphicsShape *_parentShape;
                int _index;

            protected:
                virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

            public:
                AnnoGraphicsControlPoint(AnnoGraphicsShape *parent, int index);
                virtual ~AnnoGraphicsControlPoint();

            public:
                void setupAppearance();
                void setIndex(int index);
                int getIndex();
        };

    }
}

#endif /*ANNOGRAPHICSCONTROLPOINT_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
