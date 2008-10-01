#ifndef ANNOGRAPHICSPIXMAP_H_
#define ANNOGRAPHICSPIXMAP_H_

#include <QGraphicsPixmapItem>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;
class QPixmap;

namespace anno {
    namespace graphics {

        class AnnoGraphicsPixmap : public QGraphicsPixmapItem {
                // internal mouse interface
            protected:
                virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
                virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
                virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
                virtual void keyPressEvent(QKeyEvent *event);
                virtual void keyReleaseEvent(QKeyEvent *event);

            public:
                AnnoGraphicsPixmap(const QPixmap &pixmap, QGraphicsItem *parent = 0);
                virtual ~AnnoGraphicsPixmap();

                // external mouse interface
            public:
                void exMouseMoveEvent(QGraphicsSceneMouseEvent *event);
                void exMousePressEvent(QGraphicsSceneMouseEvent *event);
                void exDoubleClickEvent(QGraphicsSceneMouseEvent *event);
                void exMouseReleaseEvent(QGraphicsSceneMouseEvent *event);

        };

    }
}

#endif /*ANNOGRAPHICSPIXMAP_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
