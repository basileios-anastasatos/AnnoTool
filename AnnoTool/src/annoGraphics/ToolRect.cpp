#include "include/ToolRect.h"
#include <QGraphicsSceneMouseEvent>
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoRectangle.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "annotoolmainwindow.h"

namespace anno {
    namespace graphics {

        ToolRect::ToolRect(AnnoGraphicsScene *scene) :
            GraphicsTool(scene) {
            _curShape = NULL;
        }

        ToolRect::~ToolRect() {
        }

        bool ToolRect::handlesCp() const {
            return false;
        }

        bool ToolRect::handlesShape() const {
            return false;
        }

        bool ToolRect::handlesImage() const {
            return true;
        }

        void ToolRect::mousePressEvent(AnnoGraphicsControlPoint *cp,
                                       QGraphicsSceneMouseEvent *event) {
            if (cp->parentShape() != NULL) {
                GlobalLogger::instance()->logDebug("Tool-Rect: CP: mousePressEvent");
                //cp->parentShape()->exMousePressEvent(event);
                //TODO This must be bug-fixed!
            }
        }

        void ToolRect::mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                         QGraphicsSceneMouseEvent *event) {
            if (cp->parentShape() != NULL) {
                GlobalLogger::instance()->logDebug("Tool-Rect: CP: mouseReleaseEvent");
                //				cp->parentShape()->exMouseReleaseEvent(event);
                //TODO This must be bug-fixed!
            }
        }

        void ToolRect::mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                      QGraphicsSceneMouseEvent *event) {
            if (cp->parentShape() != NULL) {
                GlobalLogger::instance()->logDebug("Tool-Rect: CP: mouseMoveEvent");
                //				cp->parentShape()->exMouseMoveEvent(event);
                //TODO This must be bug-fixed!
            }
        }

        void ToolRect::mousePressEvent(AnnoGraphicsShape *shape,
                                       QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMousePressEvent(event);
            }
        }

        void ToolRect::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                         QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMouseReleaseEvent(event);
            }
        }

        void ToolRect::mouseMoveEvent(AnnoGraphicsShape *shape,
                                      QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMouseMoveEvent(event);
            }
        }

        void ToolRect::mousePressEvent(AnnoGraphicsPixmap *img,
                                       QGraphicsSceneMouseEvent *event) {
            event->accept();
            dt::AnnoFileData *annoFile = GlobalProjectManager::instance()->selectedFile();
            dt::AnnoRectangle *arect = new dt::AnnoRectangle();
            arect->setTopLeft(img->mapFromScene(event->scenePos()));
            arect->setSize(QSizeF(0.0, 0.0));
            dt::Annotation *anno = new dt::Annotation();
            anno->setAnnoId(QUuid::createUuid());
            anno->setShape(arect);
            annoFile->addAnnotation(anno);

            AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(anno);
            if (s != NULL) {
                _scene->addAnnoShape(s);
                _curShape = s;
                _scene->clearSelection();
                _curShape->graphicsItem()->setSelected(true);
            }

            AnnoToolMainWindow::updateUI();
        }

        void ToolRect::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                         QGraphicsSceneMouseEvent *event) {
            _curShape->cpMouseReleaseEvent(2, event);
            _curShape = NULL;
        }

        void ToolRect::mouseMoveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneMouseEvent *event) {
            if (_curShape != NULL) {
                _curShape->cpMouseMoveEvent(2, event);
            }
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
