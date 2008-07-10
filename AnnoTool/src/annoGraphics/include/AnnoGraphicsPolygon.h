#ifndef ANNOGRAPHICSPOLYGON_H_
#define ANNOGRAPHICSPOLYGON_H_

#include <QGraphicsItem>
#include "AnnoGraphicsShape.h"
#include "AnnoPolygon.h"

class QGraphicsSceneHoverEvent;

namespace anno {
    namespace graphics {

        class AnnoGraphicsPolygon : public QGraphicsPolygonItem, public AnnoGraphicsShape {
            private:
                bool _drawClosed;

            private:
                void setupAppearance();
                dt::AnnoPolygon *annoPolygon();
                const dt::AnnoPolygon *annoPolygon() const;

            protected:
                virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
                virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

            public:
                AnnoGraphicsPolygon(dt::Annotation *anno, QGraphicsItem *parent = 0);
                virtual ~AnnoGraphicsPolygon();

                // AnnoGraphicsShape Interface
            public:
                virtual void initControlPoints();
                void validateCpPos();
                virtual QGraphicsItem *graphicsItem();
                virtual void shapeMoveBy(qreal deltaX, qreal deltaY);
                virtual void shapeSizeBy(qreal facX, qreal facY);

                // Graphics Item Interface
            public:
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

                // additional shape manipulations & attributes
            public:
                void appendPolygonPoint(const QPointF &p);
                void setClosedDrawing(bool closed = true);
        };

    }
}

#endif /*ANNOGRAPHICSPOLYGON_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
