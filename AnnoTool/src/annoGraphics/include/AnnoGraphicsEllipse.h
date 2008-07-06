#ifndef ANNOGRAPHICSELLIPSE_H_
#define ANNOGRAPHICSELLIPSE_H_

#include <QGraphicsItem>
#include "AnnoGraphicsShape.h"
#include "AnnoEllipse.h"

class QGraphicsSceneHoverEvent;

namespace anno {
    namespace graphics {

        class AnnoGraphicsEllipse : public QGraphicsItem, public AnnoGraphicsShape {
            private:
                void setupAppearance();
                dt::AnnoEllipse *annoEllipse();
                const dt::AnnoEllipse *annoEllipse() const;
                QRectF mapRectToParent(const QRectF &r) const;

            protected:
                virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

            public:
                AnnoGraphicsEllipse(dt::Annotation *anno, QGraphicsItem *parent = 0);
                AnnoGraphicsEllipse(dt::Annotation *anno, const QRectF &rect,
                                    QGraphicsItem *parent = 0);
                virtual ~AnnoGraphicsEllipse();

                // AnnoGraphicsShape Interface
            public:
                virtual void initControlPoints();
                void validateCpPos();
                virtual QGraphicsItem *graphicsItem();
                virtual void shapeMoveBy(qreal deltaX, qreal deltaY);
                virtual void shapeSizeBy(qreal facX, qreal facY);

                // Graphics Item Interface
            public:
                virtual QRectF boundingRect() const;
                virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                   QWidget *widget = 0);

            public:
                virtual void cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event);
                virtual void cpMouseReleaseEvent(int index, QGraphicsSceneMouseEvent *event);
                virtual void cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event);

                // external mouse interface
            public:
                virtual void exMouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void exMousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void exMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        };

    }
}

#endif /*ANNOGRAPHICSELLIPSE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
