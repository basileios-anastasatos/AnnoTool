#include "include/ToolEllipse.h"
#include <QGraphicsSceneMouseEvent>
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoEllipse.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "annotoolmainwindow.h"


namespace anno {
    namespace graphics {

        ToolEllipse::ToolEllipse(AnnoGraphicsScene *scene) :
            GraphicsTool(scene), _curShape(NULL) {
        }

        ToolEllipse::~ToolEllipse() {
        }

        bool ToolEllipse::handlesCp() const {
            return false;
        }

        bool ToolEllipse::handlesShape() const {
            return false;
        }

        bool ToolEllipse::handlesImage() const {
            return true;
        }

        void ToolEllipse::mousePressEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneMouseEvent *event) {
            if (cp->parentShape() != NULL) {
                GlobalLogger::instance()->logDebug("Tool-Ellipse: CP: mousePressEvent");
                //cp->parentShape()->exMousePressEvent(event);
                //TODO This must be bug-fixed!
            }
        }

        void ToolEllipse::mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                            QGraphicsSceneMouseEvent *event) {
            if (cp->parentShape() != NULL) {
                GlobalLogger::instance()->logDebug("Tool-Ellipse: CP: mouseReleaseEvent");
                //				cp->parentShape()->exMouseReleaseEvent(event);
                //TODO This must be bug-fixed!
            }
        }

        void ToolEllipse::mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                         QGraphicsSceneMouseEvent *event) {
            if (cp->parentShape() != NULL) {
                GlobalLogger::instance()->logDebug("Tool-Ellipse: CP: mouseMoveEvent");
                //				cp->parentShape()->exMouseMoveEvent(event);
                //TODO This must be bug-fixed!
            }
        }

        void ToolEllipse::mousePressEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMousePressEvent(event);
            }
        }

        void ToolEllipse::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                            QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMouseReleaseEvent(event);
            }
        }

        void ToolEllipse::mouseMoveEvent(AnnoGraphicsShape *shape,
                                         QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMouseMoveEvent(event);
            }
        }

        void ToolEllipse::mousePressEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("Tool-Ellipse: MousePress");
            event->accept();
            dt::AnnoFileData *annoFile = GlobalProjectManager::instance()->selectedFile();
            dt::AnnoEllipse *ae = new dt::AnnoEllipse();
            ae->setTopLeft(img->mapFromScene(event->scenePos()));
            ae->setSize(QSizeF(0.0, 0.0));
            dt::Annotation *anno = new dt::Annotation();
            anno->setAnnoId(QUuid::createUuid());
            anno->setShape(ae);
            annoFile->addAnnotation(anno);

            AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(anno);
            if (s != NULL) {
                GlobalLogger::instance()->logDebug("Tool-Ellipse: Adding GraphicsShape");
                _scene->addAnnoShape(s);
                _curShape = s;
                _scene->clearSelection();
                _curShape->graphicsItem()->setSelected(true);
                //TODO selection fixen!
            } else {
                GlobalLogger::instance()->logError("Tool-Ellipse: invalid graphics shape");
            }

            AnnoToolMainWindow::updateUI();
        }

        void ToolEllipse::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                            QGraphicsSceneMouseEvent *event) {
            _curShape->cpMouseReleaseEvent(2, event);
            _curShape = NULL;
        }

        void ToolEllipse::mouseMoveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneMouseEvent *event) {
            if (_curShape != NULL) {
                _curShape->cpMouseMoveEvent(2, event);
            }
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
