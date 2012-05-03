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
                int _curMinZ;
                int _curMaxZ;

            public:
                // MA: switched to using pointed to fix crash after switching to Qt 4.6
                //static AnnoGraphicsScene EmptyScene;

            private:
                static AnnoGraphicsScene *EmptyScene;

                void initScene();

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

                /* MA: */
                AnnoGraphicsShape *getShapeByAnnoId(const QUuid &annoId) const;

                /** would be a better implementation, but it crashes in the end -> need to find out where to deallocate emptyScene */
                /* static AnnoGraphicsScene &getEmptyScene() { */
                /*   static AnnoGraphicsScene emptyScene; */

                /*   return emptyScene; */
                /* } */

                static AnnoGraphicsScene *getEmptyScene() {
                    if (EmptyScene == 0) {
                        EmptyScene = new AnnoGraphicsScene();
                    }
                    return EmptyScene;
                }


            public slots:
                void selectShape(const QUuid &annoId);
                void bringToFront(const QUuid &annoId);
                void bringToBack(const QUuid &annoId);
                void bringSelShapeToFront();
                void bringSelShapeToBack();
                void setShapeVisible(const QUuid &annoId, bool visible);

                void removeAnnoShape(QUuid annoId);
                void removeAllAnnoShapes();
                void addAnnoShape(::anno::dt::Annotation *anno);

                void applyColoring();

        };

    }
}

#endif /*ANNOGRAPHICSSCENE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
