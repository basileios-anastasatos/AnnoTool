#ifndef ANNOGRAPHICSPIXMAP_H_
#define ANNOGRAPHICSPIXMAP_H_

#include <QGraphicsPixmapItem>

class QGraphicsSceneMouseEvent;
class QPixmap;

namespace anno {
    namespace graphics {

        class AnnoGraphicsPixmap : public QGraphicsPixmapItem {
            protected:
                virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

            public:
                AnnoGraphicsPixmap(const QPixmap &pixmap, QGraphicsItem *parent = 0);
                virtual ~AnnoGraphicsPixmap();

        };

    }
}

#endif /*ANNOGRAPHICSPIXMAP_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
