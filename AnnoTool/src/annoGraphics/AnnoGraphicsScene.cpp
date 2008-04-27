#include "include/AnnoGraphicsScene.h"
#include "include/AnnoGraphicsShape.h"

namespace anno {
    namespace graphics {

        AnnoGraphicsScene::AnnoGraphicsScene(QObject *parent) :
            QGraphicsScene(parent) {
            _image = NULL;
            _selShape = NULL;
        }

        AnnoGraphicsScene::AnnoGraphicsScene(const QRectF &sceneRect,
                                             QObject *parent) :
            QGraphicsScene(sceneRect, parent) {
            _image = NULL;
        }

        AnnoGraphicsScene::AnnoGraphicsScene(qreal x, qreal y, qreal width,
                                             qreal height, QObject *parent) :
            QGraphicsScene(x, y, width, height, parent) {
            _image = NULL;
        }

        AnnoGraphicsScene::~AnnoGraphicsScene() {
        }

        void AnnoGraphicsScene::setAnnoImage(const QImage &image) {
            if (_image != NULL) {
                removeItem(_image);
                delete _image;
            }

            _image = new AnnoGraphicsPixmap(QPixmap::fromImage(image));
            addItem(_image);
            invalidate();
        }

        QSize AnnoGraphicsScene::annoImageSize() const {
            return _image->pixmap().size();
        }

        AnnoGraphicsPixmap *AnnoGraphicsScene::annoPixmap() {
            return _image;
        }

        void AnnoGraphicsScene::addAnnoShape(AnnoGraphicsShape *shape) {
            if (shape != NULL && _image != NULL && shape->relatedAnno() != NULL) {
                _shapes.insert(shape->relatedAnno()->annoId(), shape);
                QGraphicsItem *gi = shape->graphicsItem();
                gi->setParentItem(_image);
                shape->setParentImage(_image);
                _image->update();
            }
        }

        void AnnoGraphicsScene::removeAnnoShape(AnnoGraphicsShape *shape) {
            if (shape != NULL && _image != NULL && shape->relatedAnno() != NULL) {
                removeAnnoShape(shape->relatedAnno()->annoId());
            }
        }

        void AnnoGraphicsScene::removeAnnoShape(const QUuid &annoId) {
            if (_image != NULL && !_shapes.isEmpty()) {
                AnnoGraphicsShape *s = _shapes.value(annoId, NULL);
                if (s != NULL) {
                    s->graphicsItem()->setParentItem(NULL);
                    _shapes.remove(annoId);
                    delete s;
                }
            }
        }

        void AnnoGraphicsScene::selectShape(const QUuid &annoId) {
            clearSelection();
            AnnoGraphicsShape *s = _shapes.value(annoId, NULL);
            if (s != NULL) {
                s->graphicsItem()->setSelected(true);
            }
        }

        bool AnnoGraphicsScene::isEmpty() const {
            return _shapes.isEmpty();
        }

        bool AnnoGraphicsScene::hasImage() const {
            return (_image != NULL);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
