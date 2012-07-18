#pragma once

#include <QGraphicsRectItem>
#include "AnnoGraphicsShape.h"
#include "AnnoBoundingBox.h"

class QGraphicsSceneHoverEvent;

namespace anno {
    namespace graphics {

        class AnnoGraphicsBoundingBox : public QGraphicsRectItem, public AnnoGraphicsShape {
            private:
                void setupAppearance();
                dt::AnnoBoundingBox *annoBoundingBox();
                const dt::AnnoBoundingBox *annoBoundingBox() const;
                QRectF mapRectToParent(const QRectF &r) const;

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
                AnnoGraphicsBoundingBox(dt::Annotation *anno, QGraphicsItem *parent = 0);
                virtual ~AnnoGraphicsBoundingBox();

            public:
                virtual void initControlPoints();
                void validateCpPos();
                virtual QGraphicsItem *graphicsItem();
                virtual void shapeMoveBy(qreal deltaX, qreal deltaY);
                virtual void shapeSizeBy(qreal facX, qreal facY);
                virtual dt::AnnoShapeType shapeType() const;
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
            private:
                bool _bShowMask;
        };

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
