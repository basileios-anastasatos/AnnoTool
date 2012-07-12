#pragma once

#include <QGraphicsItem>
#include <QPainterPath>
#include "AnnoGraphicsShape.h"
#include "AnnoBrush.h"

class QGraphicsSceneHoverEvent;

namespace anno {
    namespace graphics {

        class AnnoGraphicsBrush : public QGraphicsPolygonItem, public AnnoGraphicsShape {
            private:
                bool _drawClosed;

            private:
                void setupAppearance();
                dt::AnnoBrush *annoPolygon();
                const dt::AnnoBrush *annoPolygon() const;

            protected:
                virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);
                virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
                virtual void keyPressEvent(QKeyEvent *event);
                virtual void keyReleaseEvent(QKeyEvent *event);
                virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

            public:
                AnnoGraphicsBrush(dt::Annotation *anno, QGraphicsItem *parent = 0);
                virtual ~AnnoGraphicsBrush();

                // AnnoGraphicsShape Interface
            public:
                virtual void initControlPoints();
                void validateCpPos();
                virtual QGraphicsItem *graphicsItem();
                virtual void shapeMoveBy(qreal deltaX, qreal deltaY);
                virtual void shapeSizeBy(qreal facX, qreal facY);
                virtual dt::AnnoShapeType shapeType() const;

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
                void insertPolygonPoint(const QPointF &p);
                void removePolygonPoint(int index);
                void setClosedDrawing(bool closed = true);
        };

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
