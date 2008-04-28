#include "include/AnnoGraphicsScene.h"
#include "include/AnnoGraphicsShape.h"
#include "importGlobals.h"

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
            GlobalLogger::instance()->logDebug("AGS: setting image.");
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
            GlobalLogger::instance()->logDebug("AGS: attempt of adding annotation shape.");
            if (shape != NULL && _image != NULL && shape->relatedAnno() != NULL) {
                _shapes.insert(shape->relatedAnno()->annoId(), shape);
                QGraphicsItem *gi = shape->graphicsItem();
                gi->setParentItem(_image);
                shape->setParentImage(_image);
                GlobalLogger::instance()->logDebug("AGS: added shape.");
                _image->update();
                GlobalLogger::instance()->logDebug("AGS: updated image after shape insertion.");
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
                    GlobalLogger::instance()->logDebug(QString("AGS: removing shape %1.").arg(annoId));
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
                GlobalLogger::instance()->logDebug(QString("AGS: selecting shape %1.").arg(annoId));
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
