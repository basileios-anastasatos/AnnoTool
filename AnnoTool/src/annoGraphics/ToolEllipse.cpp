#include "include/ToolEllipse.h"
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoEllipse.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "AnnoToolMainWindow.h"


namespace anno {
    namespace graphics {

        ToolEllipse::ToolEllipse(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene), _curShape(NULL) {
            _cursorNormal = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curEllipse")), 0, 0);
            _cursorActive = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curEllipse_active")), 0, 0);;
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
            if (_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorActive);
            }

            GlobalLogger::instance()->logDebug("Tool-Ellipse: MousePress");
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
                GlobalProjectManager::instance()->setSelectedAnnoRow(anno->annoId());
            } else {
                GlobalLogger::instance()->logError("Tool-Ellipse: invalid graphics shape");
            }

            AnnoToolMainWindow::updateUI();
        }

        void ToolEllipse::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                            QGraphicsSceneMouseEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_prevCursors.pop());
            }
            _curShape->cpMouseReleaseEvent(2, event);
            _curShape = NULL;
        }

        void ToolEllipse::mouseMoveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneMouseEvent *event) {
            if (_curShape != NULL) {
                _curShape->cpMouseMoveEvent(2, event);
            }
        }

        void ToolEllipse::hoverEnterEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_cursorNormal);
            }
        }

        void ToolEllipse::hoverLeaveEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(Qt::ArrowCursor);
            }
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
