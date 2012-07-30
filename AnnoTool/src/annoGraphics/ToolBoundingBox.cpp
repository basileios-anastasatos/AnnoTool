#include "include/ToolBoundingBox.h"
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Segmentation.h"
#include "AnnoBoundingBox.h"
#include "AnnoGraphicsBoundingBox.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "AnnoToolMainWindow.h"

namespace anno {
    namespace graphics {

        ToolBoundingBox::ToolBoundingBox(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene), _curShape(NULL), _curParentAnno(NULL) {
            _cursorNormal = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curBoundingBox")), 0, 0);
            _cursorActive = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curBoundingBox_active")), 0, 0);
        }

        ToolBoundingBox::~ToolBoundingBox() {
        }

        bool ToolBoundingBox::handlesCp() const {
            return false;
        }

        bool ToolBoundingBox::handlesShape() const {
            return false;
        }

        bool ToolBoundingBox::handlesImage() const {
            return true;
        }

        void ToolBoundingBox::escape() {
            _scene->removeAnnoShape(_curShape);
            _curShape = NULL;
            _curParentAnno = NULL;
        }

        void ToolBoundingBox::handleEscape(QKeyEvent *event) {
            if (event->key() == Qt::Key_Escape && _curShape != NULL && _scene != NULL) {
                escape();
            }
        }

        void ToolBoundingBox::switchDefaultTool() {
            if (_curShape != NULL && _scene != NULL) {
                escape();
            }
            GlobalToolManager::instance()->selectToolDefault();
        }

        void ToolBoundingBox::toolDeactivate() {
            if(_curShape != NULL) {
                escape();
            }
        }

        void ToolBoundingBox::mousePressEvent(AnnoGraphicsShape *shape,
                                              QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMousePressEvent(event);
            }
        }

        void ToolBoundingBox::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                                QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMouseReleaseEvent(event);
            }
        }

        void ToolBoundingBox::mouseMoveEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMouseMoveEvent(event);
            }
        }

        void ToolBoundingBox::mousePressEvent(AnnoGraphicsPixmap *img,
                                              QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorActive);
            }

            dt::AnnoBoundingBox *arect = new dt::AnnoBoundingBox();
            arect->setTopLeft(img->mapFromScene(event->scenePos()));
            arect->setSize(QSizeF(0.0, 0.0));
            QUuid parentId = GlobalToolManager::instance()->getLockedAnno();
            dt::Segmentation *segm = new dt::Segmentation();
            segm->setAnnoId(QUuid::createUuid());
            segm->setShape(arect);
            if(!parentId.isNull()) {
                _curParentAnno = GlobalProjectManager::instance()->selectedFile()->getAnnotation(parentId);
                segm->setAnnoParent(parentId);
            }

            AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(segm);
            if (s != NULL) {
                _scene->addAnnoShape(s);
                _scene->setFocusItem(s->graphicsItem());
                _scene->selectShape(segm->annoId());
                _curShape = static_cast<AnnoGraphicsBoundingBox *>(s);
            }
        }

        void ToolBoundingBox::mouseReleaseEvent(AnnoGraphicsPixmap *img,
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
                dt::Segmentation *segm = dynamic_cast<dt::Segmentation *>(_curShape->relatedAnno());
                if(_curParentAnno != NULL) {
                    _curParentAnno->addAnnoChild(segm->annoId());
                }
                curFile->addAnnotation(segm);
                GlobalProjectManager::instance()->setSelectedAnnoRow(segm->annoId());
                AnnoToolMainWindow::updateUI();
                _curShape = NULL;
            }
            _curParentAnno = NULL;
        }

        void ToolBoundingBox::mouseMoveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneMouseEvent *event) {
            if (_curShape != NULL) {
                _curShape->cpMouseMoveEvent(2, event);
            }
        }

        void ToolBoundingBox::hoverEnterEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_cursorNormal);
            }
        }

        void ToolBoundingBox::hoverLeaveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(Qt::ArrowCursor);
            }
        }

        void ToolBoundingBox::keyReleaseEvent(AnnoGraphicsControlPoint *cp, QKeyEvent *event) {
            handleEscape(event);
        }

        void ToolBoundingBox::keyReleaseEvent(AnnoGraphicsShape *shape, QKeyEvent *event) {
            handleEscape(event);
        }

        void ToolBoundingBox::keyReleaseEvent(AnnoGraphicsPixmap *img, QKeyEvent *event) {
            handleEscape(event);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
