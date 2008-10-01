#ifndef ANNOGRAPHICSSCENE_H_
#define ANNOGRAPHICSSCENE_H_

#include <QGraphicsScene>
#include <QMap>
#include <QUuid>
#include <QSize>
#include "AnnoGraphicsPixmap.h"
#include "Annotation.h"

namespace anno {
    namespace graphics {
        class AnnoGraphicsShape;

        class AnnoGraphicsScene : public QGraphicsScene {
                Q_OBJECT

            private:
                AnnoGraphicsPixmap *_image;
                AnnoGraphicsShape *_selShape;
                QMap<QUuid, AnnoGraphicsShape *> _shapes;
                int _curMaxZ;

            public:
                AnnoGraphicsScene(QObject *parent = 0);
                AnnoGraphicsScene(const QRectF &sceneRect, QObject *parent = 0);
                AnnoGraphicsScene(qreal x, qreal y, qreal width, qreal height,
                                  QObject *parent = 0);
                virtual ~AnnoGraphicsScene();

                // mouse interface
            protected:
                virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

            public:
                void setAnnoImage(const QImage &image);
                QSize annoImageSize() const;
                AnnoGraphicsPixmap *annoPixmap();
                void addAnnoShape(AnnoGraphicsShape *shape);
                void removeAnnoShape(AnnoGraphicsShape *shape);
                bool isShapeVisible(const QUuid &annoId) const;
                bool isEmpty() const;
                bool hasImage() const;

            public slots:
                void selectShape(const QUuid &annoId);
                void bringToFront(const QUuid &annoId);
                void bringSelShapeToFront();
                void setShapeVisible(const QUuid &annoId, bool visible);

                void removeAnnoShape(QUuid annoId);
                void addAnnoShape(::anno::dt::Annotation *anno);

        };

    }
}

#endif /*ANNOGRAPHICSSCENE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
