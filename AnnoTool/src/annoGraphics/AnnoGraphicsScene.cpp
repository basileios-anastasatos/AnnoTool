#include "include/AnnoGraphicsScene.h"
#include "include/AnnoGraphicsShape.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"

namespace anno {
    namespace graphics {

        AnnoGraphicsScene::AnnoGraphicsScene(QObject *parent) :
            QGraphicsScene(parent), _image(NULL), _selShape(NULL), _curMinZ(0), _curMaxZ(0) {
            initScene();
        }

        AnnoGraphicsScene::AnnoGraphicsScene(const QRectF &sceneRect, QObject *parent) :
            QGraphicsScene(sceneRect, parent), _image(NULL), _selShape(NULL), _curMinZ(0), _curMaxZ(0) {
            initScene();
        }

        AnnoGraphicsScene::AnnoGraphicsScene(qreal x, qreal y, qreal width, qreal height,
                                             QObject *parent) :
            QGraphicsScene(x, y, width, height, parent), _image(NULL), _selShape(NULL), _curMinZ(0), _curMaxZ(0) {
            initScene();
        }

        AnnoGraphicsScene::~AnnoGraphicsScene() {
        }

        void AnnoGraphicsScene::initScene() {
//			setBackgroundBrush(Qt::red);
        }

        void AnnoGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AGS: mouseDoubleClickEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseDoubleClickEvent(event);
            }
            QGraphicsScene::mouseDoubleClickEvent(event);
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

        void AnnoGraphicsScene::removeAnnoShape(QUuid annoId) {
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

        void AnnoGraphicsScene::addAnnoShape(::anno::dt::Annotation *anno) {
            anno::graphics::AnnoGraphicsShape *s =
                anno::graphics::AnnoGraphicsShapeCreator::toGraphicsShape(anno);
            if (s != NULL) {
                addAnnoShape(s);
            }
        }

        void AnnoGraphicsScene::selectShape(const QUuid &annoId) {
            clearSelection();
            AnnoGraphicsShape *s = _shapes.value(annoId, NULL);
            if (s != NULL) {
                GlobalLogger::instance()->logDebug(QString("AGS: selecting shape %1.").arg(annoId));
                s->graphicsItem()->setSelected(true);
                _selShape = s;
            }
        }

        void AnnoGraphicsScene::bringToFront(const QUuid &annoId) {
            AnnoGraphicsShape *s = _shapes.value(annoId, NULL);
            if (s != NULL) {
                GlobalLogger::instance()->logDebug(QString("AGS: bringing shape %1 to Zorder top. [z=%2]").arg(annoId).arg(_curMaxZ + 1));
                s->graphicsItem()->setZValue(++_curMaxZ);
            }
        }

        void AnnoGraphicsScene::bringToBack(const QUuid &annoId) {
            AnnoGraphicsShape *s = _shapes.value(annoId, NULL);
            if (s != NULL) {
                GlobalLogger::instance()->logDebug(QString("AGS: bringing shape %1 to Zorder back. [z=%2]").arg(annoId).arg(_curMinZ - 1));
                s->graphicsItem()->setZValue(--_curMinZ);
            }
        }

        void AnnoGraphicsScene::bringSelShapeToFront() {
            if (_selShape != NULL) {
                GlobalLogger::instance()->logDebug(QString("AGS: bringing selected shape %1 to Zorder top. [z=%2]").arg(_selShape->relatedAnno()->annoIdAsString()).arg(_curMaxZ + 1));
                _selShape->graphicsItem()->setZValue(++_curMaxZ);
                update();
            }
        }

        void AnnoGraphicsScene::bringSelShapeToBack() {
            if (_selShape != NULL) {
                GlobalLogger::instance()->logDebug(QString("AGS: bringing selected shape %1 to Zorder back. [z=%2]").arg(_selShape->relatedAnno()->annoIdAsString()).arg(_curMinZ - 1));
                _selShape->graphicsItem()->setZValue(--_curMinZ);
                update();
            }
        }

        void AnnoGraphicsScene::setShapeVisible(const QUuid &annoId, bool visible) {
            AnnoGraphicsShape *s = _shapes.value(annoId, NULL);
            if (s != NULL) {
                s->graphicsItem()->setVisible(visible);
            }
        }

        bool AnnoGraphicsScene::isShapeVisible(const QUuid &annoId) const {
            AnnoGraphicsShape *s = _shapes.value(annoId, NULL);
            if (s != NULL) {
                return s->graphicsItem()->isVisible();
            }
            return false;
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
