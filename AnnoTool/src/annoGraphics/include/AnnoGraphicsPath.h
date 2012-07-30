#pragma once

#include <QGraphicsPathItem>
#include <QPainterPath>
#include "AnnoGraphicsShape.h"
#include "AnnoPath.h"

class QGraphicsSceneHoverEvent;

namespace anno {
    namespace graphics {

        class AnnoGraphicsPath : public QGraphicsPathItem, public AnnoGraphicsShape {
            private:
                bool		 _bIsForeground;
                QPainterPath _painterPath;

            private:
                void setupAppearance();
                dt::AnnoPath *annoPath();
                const dt::AnnoPath *annoPath() const;

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
                AnnoGraphicsPath(dt::Annotation *anno, QGraphicsItem *parent = 0);
                virtual ~AnnoGraphicsPath();

                // AnnoGraphicsShape Interface
            public:
                virtual QGraphicsItem *graphicsItem();
                virtual void shapeMoveBy(qreal deltaX, qreal deltaY);
                virtual void shapeSizeBy(qreal facX, qreal facY);
                virtual dt::AnnoShapeType shapeType() const;

                // Graphics Item Interface
            public:
                virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                   QWidget *widget = 0);

                // external mouse interface
            public:
                virtual void exMouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void exMousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void exMouseReleaseEvent(QGraphicsSceneMouseEvent *event);

                // public path methods
            public:
                void setIsForeground(bool bIsForeground) {
                    _bIsForeground = bIsForeground;
                }
                void startPathFromPoint(const QPointF &pt);
                void addPointToPath(const QPointF &pt);

        };

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
