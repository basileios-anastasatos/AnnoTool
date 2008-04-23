#ifndef ANNOGRAPHICSSCENE_H_
#define ANNOGRAPHICSSCENE_H_

#include <QGraphicsScene>
#include <QMap>
#include <QUuid>
#include "AnnoGraphicsPixmap.h"

namespace anno {
    namespace graphics {
        class AnnoGraphicsShape;

        class AnnoGraphicsScene : public QGraphicsScene {
                Q_OBJECT

            private:
                AnnoGraphicsPixmap *_image;
                AnnoGraphicsShape *_selShape;
                QMap<QUuid, AnnoGraphicsShape *> _shapes;

            public:
                AnnoGraphicsScene(QObject *parent = 0);
                AnnoGraphicsScene(const QRectF &sceneRect, QObject *parent = 0);
                AnnoGraphicsScene(qreal x, qreal y, qreal width, qreal height,
                                  QObject *parent = 0);
                virtual ~AnnoGraphicsScene();

            public:
                void setAnnoImage(const QImage &image);
                void addAnnoShape(AnnoGraphicsShape *shape);
                void removeAnnoShape(AnnoGraphicsShape *shape);
                void removeAnnoShape(const QUuid &annoId);
                void selectShape(const QUuid &annoId);
                bool isEmpty() const;
                bool hasImage() const;
        };

    }
}

#endif /*ANNOGRAPHICSSCENE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
