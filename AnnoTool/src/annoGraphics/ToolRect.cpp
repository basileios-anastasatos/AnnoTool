#include "include/ToolRect.h"
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoRectangle.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsRect.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "AnnoToolMainWindow.h"

namespace anno {
    namespace graphics {

        ToolRect::ToolRect(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene), _curShape(NULL), _curParentAnno(NULL) {
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

        void ToolRect::escape() {
            _scene->removeAnnoShape(_curShape);
            _curShape = NULL;
            _curParentAnno = NULL;
        }

        void ToolRect::handleEscape(QKeyEvent *event) {
            if (event->key() == Qt::Key_Escape && _curShape != NULL && _scene != NULL) {
                escape();
            }
        }

        void ToolRect::switchDefaultTool() {
            if (_curShape != NULL && _scene != NULL) {
                escape();
            }
            GlobalToolManager::instance()->selectToolDefault();
        }

        void ToolRect::toolDeactivate() {
            if(_curShape != NULL) {
                escape();
            }
        }

        void ToolRect::mousePressEvent(AnnoGraphicsShape *shape,
                                       QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMousePressEvent(event);
            }
        }

        void ToolRect::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                         QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

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
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorActive);
            }

            dt::AnnoRectangle *arect = new dt::AnnoRectangle();
            arect->setTopLeft(img->mapFromScene(event->scenePos()));
            arect->setSize(QSizeF(0.0, 0.0));
            QUuid parentId = GlobalToolManager::instance()->getLockedAnno();
            dt::Annotation *anno = new dt::Annotation();
            anno->setAnnoId(QUuid::createUuid());
            anno->setShape(arect);
            if(!parentId.isNull()) {
                _curParentAnno = GlobalProjectManager::instance()->selectedFile()->getAnnotation(parentId);
                anno->setAnnoParent(parentId);
            }

            AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(anno);
            if (s != NULL) {
                _scene->addAnnoShape(s);
                _scene->setFocusItem(s->graphicsItem());
                _scene->selectShape(anno->annoId());
                _curShape = static_cast<AnnoGraphicsRect *>(s);
            }
        }

        void ToolRect::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                         QGraphicsSceneMouseEvent *event) {
            if (event->button() == Qt::RightButton) {
                switchDefaultTool();
                return;
            } else if (event->button() != Qt::LeftButton) {
                return;
            }

            if (_view != NULL) {
                _view->setCursor(_prevCursors.pop());
            }
            if (_curShape != NULL) {
                _curShape->cpMouseReleaseEvent(2, event);
                dt::AnnoFileData *curFile = GlobalProjectManager::instance()->selectedFile();
                dt::Annotation *anno = _curShape->relatedAnno();
                if(_curParentAnno != NULL) {
                    _curParentAnno->addAnnoChild(anno->annoId());
                }
                curFile->addAnnotation(anno);
                GlobalProjectManager::instance()->setSelectedAnnoRow(anno->annoId());
                AnnoToolMainWindow::updateUI();
                _curShape = NULL;
            }
            _curParentAnno = NULL;
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

        void ToolRect::keyReleaseEvent(AnnoGraphicsControlPoint *cp, QKeyEvent *event) {
            handleEscape(event);
        }

        void ToolRect::keyReleaseEvent(AnnoGraphicsShape *shape, QKeyEvent *event) {
            handleEscape(event);
        }

        void ToolRect::keyReleaseEvent(AnnoGraphicsPixmap *img, QKeyEvent *event) {
            handleEscape(event);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
