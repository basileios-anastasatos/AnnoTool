#ifndef ANNOGRAPHICSPIXMAP_H_
#define ANNOGRAPHICSPIXMAP_H_

#include <QGraphicsPixmapItem>

class QGraphicsSceneMouseEvent;
class AnnoGraphicsPolygon;

class AnnoGraphicsPixmap : public QObject, public QGraphicsPixmapItem {
        Q_OBJECT

    private:
        QVector<AnnoGraphicsPolygon *> _polygons;

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    public:
        AnnoGraphicsPixmap(const QPixmap &pixmap, QGraphicsItem *parent = 0);
        virtual ~AnnoGraphicsPixmap();
};

#endif /*ANNOGRAPHICSPIXMAP_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
