#ifndef ANNOGRAPHICSSINGLEPOINT_H_
#define ANNOGRAPHICSSINGLEPOINT_H_

#include <QGraphicsItem>
#include "AnnoGraphicsShape.h"
#include "AnnoSinglePoint.h"

class QGraphicsSceneHoverEvent;

namespace anno {

    namespace graphics {

        class AnnoGraphicsSinglePoint : public QGraphicsItem, public AnnoGraphicsShape {
            private:
                void setupAppearance();
                dt::AnnoSinglePoint *annoSinglePoint();
                const dt::AnnoSinglePoint *annoSinglePoint() const;

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
                AnnoGraphicsSinglePoint(dt::Annotation *anno, QGraphicsItem *parent = 0);
                AnnoGraphicsSinglePoint(dt::Annotation *anno, const QPointF &point,
                                        QGraphicsItem *parent = 0);
                AnnoGraphicsSinglePoint(dt::Annotation *anno, qreal x, qreal y,
                                        QGraphicsItem *parent = 0);
                virtual ~AnnoGraphicsSinglePoint();

            public:
                QRectF mapPointToParent(const QPointF &p) const;

                // AnnoGraphicsShape Interface
            public:
                virtual QGraphicsItem *graphicsItem();
                virtual void shapeMoveBy(qreal deltaX, qreal deltaY);
                virtual void shapeSizeBy(qreal facX, qreal facY);
                virtual dt::AnnoShapeType shapeType() const;

                // Graphics Item Interface
            public:
                virtual QRectF boundingRect() const;
                virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                   QWidget *widget = 0);

                // external mouse interface
            public:
                virtual void exMouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void exMousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void exMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        };

    }

}

#endif /*ANNOGRAPHICSSINGLEPOINT_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
