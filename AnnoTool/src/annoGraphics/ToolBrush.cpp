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

            if (!_modify && _curShape != NULL) {
//				_curShape->removePolygonPoint(_curShape->polygon().size() - 1);
//				finishPolygon();
            }
        }


        void ToolBrush::mousePressEvent(AnnoGraphicsShape *shape,
                                        QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton && event->button() != Qt::MidButton) {
                return;
            }

//			if(isModifyResetEvent(event))
//			{
//				resetModify();
//			}
//
//			if(isModifyActive(event))
//			{
//				if (isType(shape))
//				{
//					AnnoGraphicsPath* poly = (AnnoGraphicsBrush*)shape;
//					poly->insertPolygonPoint(event->pos());

//					GlobalProjectManager* pm = GlobalProjectManager::instance();
//					anno::dt::Annotation* anno = pm->selectedAnno();
//					if(NULL == anno)
//						return;
//					anno::dt::Segmentation* segm = dynamic_cast<anno::dt::Segmentation*>(anno);
//					if(NULL == segm)
//						return;
//					_curParentAnno = segm;
//
//					if(_bFGBrush/*event->button() == Qt::LeftButton*/)
//					{
//						_lbDrag = true;
//						_lbStart = event->pos();
//						_painterFGPath.moveTo((int)(_lbStart.x()), (int)(_lbStart.y()));
//						return;
//					}
//					else
//					//if(event->button() == Qt::RightButton)
//					{
//						_rbDrag = true;
//						_rbStart = event->pos();
//						//_painterPath = QPainterPath();
//						_painterBGPath.moveTo((int)(_rbStart.x()), (int)(_rbStart.y()));
//						return;
//					}
//				}
//			}
//			else if(!isModifyEvent(event))
            {
                if (shape->parentImage() != NULL) {
                    shape->parentImage()->exMousePressEvent(event);
                }
            }
        }

        void ToolBrush::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton && event->button() != Qt::MidButton) {
                return;
            }

//			if (isModifyResetEvent(event))
//			{
//				resetModify();
//			}
//
//			if(isModifyEvent(event))
//			{
//				if(_lbDrag)
//				{
//					_lbDrag = false;
//
//					if (NULL == _curParentAnno)
//						return;
//					dt::Segmentation* segm = dynamic_cast<dt::Segmentation*>(_curParentAnno);
//					if (NULL == segm)
//						return;
//
//					segm->setFGPAth(_painterFGPath);
//
//					return;
//				}
//
//				if(_rbDrag)
//				{
//					_rbDrag = false;
//
//					if (NULL == _curParentAnno)
//						return;
//					dt::Segmentation* segm = dynamic_cast<dt::Segmentation*>(_curParentAnno);
//					if (NULL == segm)
//						return;
//
//					segm->setBGPAth(_painterBGPath);
//
//					return;
//				}

//				if (isType(shape) && shape->graphicsItem()->contains(event->pos()))
//				{
//					setModify(shape);
//				}
//			}
//			else
            {
                if (shape->parentImage() != NULL) {
                    shape->parentImage()->exMouseReleaseEvent(event);
                }
            }
        }

        void ToolBrush::mouseMoveEvent(AnnoGraphicsShape *shape,
                                       QGraphicsSceneMouseEvent *event) {
//			if(isModifyActive(event))
//			{
//				if (GlobalProjectManager::instance()->isAnnoSelected(shape->relatedAnno()))
//				{
//					if(_lbDrag)
//					{
//						_lbStart = event->pos();
//						_painterFGPath.lineTo((int)(_lbStart.x()), (int)(_lbStart.y()));
//						_graphicsFGPath.setPath(_painterFGPath);
//						_graphicsFGPath.show();
//					}
//
//					if(_rbDrag)
//					{
//						_rbStart = event->pos();
//						_painterBGPath.lineTo((int)(_rbStart.x()), (int)(_rbStart.y()));
//						_graphicsBGPath.setPath(_painterBGPath);
//						_graphicsBGPath.show();
//					}

//					qreal deltaX = event->pos().x() - event->lastPos().x();
//					qreal deltaY = event->pos().y() - event->lastPos().y();
//					shape->shapeMoveBy(deltaX, deltaY);
//				}
//			}
//			else if(!isModifyEvent(event))
            {
                if (shape->parentImage() != NULL) {
                    shape->parentImage()->exMouseMoveEvent(event);
                }
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

            dt::AnnoPath *aPath = new dt::AnnoPath();
//			aPath->moveTo(pt);

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
                //_curParentAnno = GlobalProjectManager::instance()->selectedFile()->getAnnotation(parentId);
                anno->setAnnoParent(parentId);
            }

            _bDrag = true;

            _bFGBrush = true;
            if(isModifyEvent(event) || event->button() == Qt::MidButton) {
                _bFGBrush = false;
            }

            if (_curShape == NULL) {
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

//			if(_modify)
//			{
//				resetModify();
//			}
//			else
//			{
//				GlobalProjectManager* pm = GlobalProjectManager::instance();
//				anno::dt::Annotation* anno = pm->selectedAnno();
//				if(NULL == anno)
//					return;
//				anno::dt::Segmentation* segm = dynamic_cast<anno::dt::Segmentation*>(anno);
//				if(NULL == segm)
//					return;
//
//				if (_curShape == NULL)
//				{
//					AnnoGraphicsShape* s = new AnnoGraphicsPath(segm);
//					if (s != NULL)
//					{
//						_scene->addAnnoShape(s);
//						_curShape = static_cast<AnnoGraphicsPath*>(s);
//						_curShape->setIsForeground(_bFGBrush);
//					}
//				}
//
//				_bDrag = true;
//				QPointF pt = img->mapFromScene(event->scenePos());
//				pt.setX(int(pt.x()));
//				pt.setY(int(pt.y()));
//				_curShape->startPathFromPoint(pt);
//				return;
//			}
        }

        void ToolBrush::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneMouseEvent *event) {
            if (event->button() == Qt::RightButton) {
                switchDefaultTool();
                return;
            } else if (event->button() != Qt::LeftButton && event->button() != Qt::MidButton) {
                return;
            }

            if (_view != NULL) {
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
                    segm->appendFGPath(_curShape->path());
                } else {
                    segm->appendBGPath(_curShape->path());
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
//			GlobalLogger::instance()->logDebug("ToolPolygon::mouseMoveEvent");

//			if (_bDrag && _curShape)
//			{
//				QPointF pt = img->mapFromScene(event->scenePos());
//				pt.setX(int(pt.x()));
//				pt.setY(int(pt.y()));
//				_curShape->addPointToPath(pt);
//			}
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
