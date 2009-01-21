#include "include/ToolEllipse.h"
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoEllipse.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsEllipse.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "AnnoToolMainWindow.h"


namespace anno {
    namespace graphics {

        ToolEllipse::ToolEllipse(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene), _curShape(NULL), _curParentAnno(NULL) {
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

        void ToolEllipse::escape() {
            _scene->removeAnnoShape(_curShape);
            _curShape = NULL;
            _curParentAnno = NULL;
        }

        void ToolEllipse::handleEscape(QKeyEvent *event) {
            if (event->key() == Qt::Key_Escape && _curShape != NULL && _scene != NULL) {
                escape();
            }
        }

        void ToolEllipse::switchDefaultTool() {
            if(_curShape != NULL && _scene != NULL) {
                escape();
            }
            GlobalToolManager::instance()->selectToolDefault();
        }

        void ToolEllipse::toolDeactivate() {
            if(_curShape != NULL) {
                escape();
            }
        }

        void ToolEllipse::mousePressEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMousePressEvent(event);
            }
        }

        void ToolEllipse::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                            QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

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
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorActive);
            }

            dt::AnnoEllipse *ae = new dt::AnnoEllipse();
            ae->setTopLeft(img->mapFromScene(event->scenePos()));
            ae->setSize(QSizeF(0.0, 0.0));
            QUuid parentId = GlobalToolManager::instance()->getLockedAnno();
            dt::Annotation *anno = new dt::Annotation();
            anno->setAnnoId(QUuid::createUuid());
            anno->setShape(ae);
            if (!parentId.isNull()) {
                _curParentAnno = GlobalProjectManager::instance()->selectedFile()->getAnnotation(parentId);
                anno->setAnnoParent(parentId);
            }

            AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(anno);
            if (s != NULL) {
                _scene->addAnnoShape(s);
                _scene->setFocusItem(s->graphicsItem());
                _scene->selectShape(anno->annoId());
                _curShape = static_cast<AnnoGraphicsEllipse *>(s);
            }
        }

        void ToolEllipse::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                            QGraphicsSceneMouseEvent *event) {
            if(event->button() == Qt::RightButton) {
                switchDefaultTool();
                return;
            } else if(event->button() != Qt::LeftButton) {
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

        void ToolEllipse::keyReleaseEvent(AnnoGraphicsControlPoint *cp, QKeyEvent *event) {
            handleEscape(event);
        }

        void ToolEllipse::keyReleaseEvent(AnnoGraphicsShape *shape, QKeyEvent *event) {
            handleEscape(event);
        }

        void ToolEllipse::keyReleaseEvent(AnnoGraphicsPixmap *img, QKeyEvent *event) {
            handleEscape(event);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
