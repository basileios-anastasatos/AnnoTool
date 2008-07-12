#include "include/ToolRect.h"
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoRectangle.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "AnnoToolMainWindow.h"

namespace anno {
    namespace graphics {

        ToolRect::ToolRect(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene), _curShape(NULL) {
            _cursorNormal = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curRect")), 0, 0);
            _cursorActive = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curRect_active")), 0, 0);
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
            if(_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorActive);
            }

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
                GlobalProjectManager::instance()->setSelectedAnnoRow(anno->annoId());
            }

            AnnoToolMainWindow::updateUI();
        }

        void ToolRect::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                         QGraphicsSceneMouseEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_prevCursors.pop());
            }
            _curShape->cpMouseReleaseEvent(2, event);
            _curShape = NULL;
        }

        void ToolRect::mouseMoveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneMouseEvent *event) {
            if (_curShape != NULL) {
                _curShape->cpMouseMoveEvent(2, event);
            }
        }

        void ToolRect::hoverEnterEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_cursorNormal);
            }
        }

        void ToolRect::hoverLeaveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(Qt::ArrowCursor);
            }
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
