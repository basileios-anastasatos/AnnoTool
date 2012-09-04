#include "include/ToolBrush.h"
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Segmentation.h"
#include "AnnoPath.h"
#include "AnnoGraphicsPath.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "AnnoToolMainWindow.h"


namespace anno {
    namespace graphics {

        ToolBrush::ToolBrush(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene), _curShape(NULL), _curParentAnno(NULL), _modify(false) {
            _cursorNormal = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curBrush")), 0, 0);
            _cursorActive = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curBrush_active")), 0, 0);
            _bFGBrush = true;
            _bDrag  = false;
        }

        ToolBrush::~ToolBrush() {
        }

        bool ToolBrush::handlesCp() const {
            return false;
        }

        bool ToolBrush::handlesShape() const {
            return false;
        }

        bool ToolBrush::handlesImage() const {
            return true;
        }

        void ToolBrush::escape() {
            _scene->removeAnnoShape(_curShape);
            _curShape = NULL;
            _curParentAnno = NULL;
        }

        void ToolBrush::handleEscape(QKeyEvent *event) {
            if (!_modify && event->key() == Qt::Key_Escape && _curShape != NULL && _scene != NULL) {
                escape();
            }
        }

        void ToolBrush::switchDefaultTool() {
            if (!_modify && _curShape != NULL && _scene != NULL) {
                escape();
            }
            GlobalToolManager::instance()->selectToolDefault();
        }

        void ToolBrush::toolDeactivate() {
            if (_curShape != NULL) {
                escape();
            }
        }

        void ToolBrush::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton && event->button() != Qt::MidButton) {
                return;
            }
        }


        void ToolBrush::mousePressEvent(AnnoGraphicsShape *shape,
                                        QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton && event->button() != Qt::MidButton) {
                return;
            }

            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMousePressEvent(event);
            }
        }

        void ToolBrush::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton && event->button() != Qt::MidButton) {
                return;
            }

            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMouseReleaseEvent(event);
            }
        }

        void ToolBrush::mouseMoveEvent(AnnoGraphicsShape *shape,
                                       QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMouseMoveEvent(event);
            }
        }

        void ToolBrush::mousePressEvent(AnnoGraphicsPixmap *img,
                                        QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton && event->button() != Qt::MidButton) {
                return;
            }

            if (_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorActive);
            }

            QPointF pt = img->mapFromScene(event->scenePos());
            pt.setX(qRound(pt.x()));
            pt.setY(qRound(pt.y()));

            dt::AnnoPath *aPath = new dt::AnnoPath(); //dummy

            GlobalProjectManager *pm = GlobalProjectManager::instance();
            anno::dt::Annotation *selAnno = pm->selectedAnno();
            if(NULL == selAnno) {
                return;
            }
            anno::dt::Segmentation *segm = dynamic_cast<anno::dt::Segmentation *>(selAnno);
            if(NULL == segm) {
                return;
            }

            _curParentAnno = segm;

            QUuid parentId = _curParentAnno->annoId();//GlobalToolManager::instance()->getLockedAnno();
            dt::Annotation *anno = new dt::Annotation();
            anno->setAnnoId(QUuid::createUuid());
            anno->setShape(aPath);
            if(!parentId.isNull()) {
                anno->setAnnoParent(parentId);
            }

            _bDrag = true;

            _bFGBrush = true;
            if(isModifyEvent(event) || event->button() == Qt::MidButton) {
                _bFGBrush = false;
            }

            AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(anno);
            if (s != NULL) {
                _scene->addAnnoShape(s);
                _scene->setFocusItem(s->graphicsItem());
                _scene->selectShape(anno->annoId());
                _curShape = static_cast<AnnoGraphicsPath *>(s);
                _curShape->setIsForeground(_bFGBrush);
                _curShape->startPathFromPoint(pt);
                //_curShape->setPath(aPath->path());
            }
        }

        void ToolBrush::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneMouseEvent *event) {
            if (event->button() == Qt::RightButton) {
                switchDefaultTool();
                return;
            } else if (event->button() != Qt::LeftButton && event->button() != Qt::MidButton) {
                return;
            }

            if (_view != NULL && !_prevCursors.isEmpty()) {
                _view->setCursor(_prevCursors.pop());
            }

            if (_curShape != NULL) {
                _curShape->cpMouseReleaseEvent(2, event);
                // commented code
//				dt::AnnoFileData* curFile = GlobalProjectManager::instance()->selectedFile();
//				dt::Annotation* anno = _curShape->relatedAnno();
//				if(_curParentAnno != NULL)
//					_curParentAnno->addAnnoChild(anno->annoId());
//				curFile->addAnnotation(anno);
//				GlobalProjectManager::instance()->setSelectedAnnoRow(anno->annoId());
//				AnnoToolMainWindow::updateUI();
                //

                dt::Segmentation *segm = dynamic_cast<dt::Segmentation *>(_curParentAnno);

                if(_bFGBrush) {
                    segm->appendFGPath(*(_curShape->getPath()));
                } else {
                    segm->appendBGPath(*(_curShape->getPath()));
                }

                segm->setModified(true);

                _curShape = NULL;
            }
            _curParentAnno = NULL;
        }

        void ToolBrush::mouseMoveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneMouseEvent *event) {
            if (_curShape != NULL) {
                QPointF pt = img->mapFromScene(event->scenePos());
                pt.setX(qRound(pt.x()));
                pt.setY(qRound(pt.y()));
                _curShape->addPointToPath(pt);
//				_curShape->cpMouseMoveEvent(2, event);
            }
        }

        void ToolBrush::hoverEnterEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_cursorNormal);
            }
        }

        void ToolBrush::hoverLeaveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(Qt::ArrowCursor);
            }
        }

        void ToolBrush::hoverEnterEvent(AnnoGraphicsShape *shape,
                                        QGraphicsSceneHoverEvent *event) {
            if (_curShape == NULL && isModifyEvent(event) && isType(shape)) {
                _curShape = (AnnoGraphicsPath *) shape;
            }
        }

        void ToolBrush::hoverLeaveEvent(AnnoGraphicsShape *shape,
                                        QGraphicsSceneHoverEvent *event) {

        }

        void ToolBrush::keyReleaseEvent(AnnoGraphicsShape *shape, QKeyEvent *event) {
            handleEscape(event);
        }

        void ToolBrush::keyReleaseEvent(AnnoGraphicsPixmap *img, QKeyEvent *event) {
            handleEscape(event);
        }


    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
