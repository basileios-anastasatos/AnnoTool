#ifndef ANNOGRAPHICSRECT_H_
#define ANNOGRAPHICSRECT_H_

#include <QGraphicsRectItem>
#include "AnnoGraphicsShape.h"
#include "AnnoRectangle.h"

class QGraphicsSceneHoverEvent;

namespace anno {
    namespace graphics {

        class AnnoGraphicsRect : public QGraphicsRectItem, public AnnoGraphicsShape {
            private:
                void setupAppearance();
                dt::AnnoRectangle *annoRect();
                QRectF mapRectToParent(const QRectF &r) const;

            protected:
                virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

            public:
                AnnoGraphicsRect(dt::Annotation *anno, QGraphicsItem *parent = 0);
                AnnoGraphicsRect(dt::Annotation *anno, const QRectF &rect,
                                 QGraphicsItem *parent = 0);
                AnnoGraphicsRect(dt::Annotation *anno, qreal x, qreal y, qreal width,
                                 qreal height, QGraphicsItem *parent = 0);
                virtual ~AnnoGraphicsRect();

            public:
                virtual void initControlPoints();
                void validateCpPos();
                virtual QGraphicsItem *graphicsItem();
                virtual void shapeMoveBy(qreal deltaX, qreal deltaY);
                virtual void shapeSizeBy(qreal facX, qreal facY);
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

#endif /*ANNOGRAPHICSRECT_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
