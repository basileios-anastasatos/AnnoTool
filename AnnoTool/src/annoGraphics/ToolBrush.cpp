#include "include/ToolBrush.h"
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsBrush.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "AnnoToolMainWindow.h"


namespace anno {
    namespace graphics {

        ToolBrush::ToolBrush(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene), _curShape(NULL), _curParentAnno(NULL), _modify(false) {
            _cursorNormal = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curPoly")), 0, 0);
            _cursorActive = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curPoly_active")), 0, 0);
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

        void ToolBrush::finishPolygon() {
            if (_curShape != NULL) {
                if(_curShape->polygon().size() > 2) {
                    dt::AnnoFileData *curFile = GlobalProjectManager::instance()->selectedFile();
                    dt::Annotation *anno = _curShape->relatedAnno();
                    if (_curParentAnno != NULL) {
                        _curParentAnno->addAnnoChild(anno->annoId());
                    }
                    curFile->addAnnotation(anno);
                    _curShape->setClosedDrawing(true);
                    _curShape->update();
                    GlobalProjectManager::instance()->setSelectedAnnoRow(anno->annoId());
                    AnnoToolMainWindow::updateUI();
                    _curShape = NULL;
                    _curParentAnno = NULL;
                } else {
                    if (!_modify && _curShape != NULL && _scene != NULL) {
                        escape();
                    }
                }
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
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (!_modify && _curShape != NULL) {
                _curShape->removePolygonPoint(_curShape->polygon().size() - 1);
                finishPolygon();
            }
        }

        void ToolBrush::mousePressEvent(AnnoGraphicsControlPoint *cp,
                                        QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if(isModifyActive(event)) {
                if((event->modifiers() & Qt::ShiftModifier) != 0) {
                    _curShape->removePolygonPoint(cp->getIndex());
                } else {
                    cp->parentShape()->cpMousePressEvent(cp->getIndex(), event);
                }
            } else if(!isModifyEvent(event) && cp->getIndex() == 0) {
                finishPolygon();
            }
        }

        void ToolBrush::mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if(isModifyActive(event)) {
                cp->parentShape()->cpMouseReleaseEvent(cp->getIndex(), event);
            }
        }

        void ToolBrush::mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                       QGraphicsSceneMouseEvent *event) {
            if(isModifyActive(event)) {
                cp->parentShape()->cpMouseMoveEvent(cp->getIndex(), event);
            }
        }

        void ToolBrush::mousePressEvent(AnnoGraphicsShape *shape,
                                        QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if(isModifyResetEvent(event)) {
                resetModify();
            }

            if(isModifyActive(event)) {
                if (isType(shape)) {
                    AnnoGraphicsBrush *poly = (AnnoGraphicsBrush *)shape;
                    poly->insertPolygonPoint(event->pos());
                }
            } else if(!isModifyEvent(event)) {
                if (shape->parentImage() != NULL) {
                    shape->parentImage()->exMousePressEvent(event);
                }
            }
        }

        void ToolBrush::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (isModifyResetEvent(event)) {
                resetModify();
            }

            if(isModifyEvent(event)) {
                if (isType(shape) && shape->graphicsItem()->contains(event->pos())) {
                    setModify(shape);
                }
            } else {
                if (shape->parentImage() != NULL) {
                    shape->parentImage()->exMouseReleaseEvent(event);
                }
            }
        }

        void ToolBrush::mouseMoveEvent(AnnoGraphicsShape *shape,
                                       QGraphicsSceneMouseEvent *event) {
            if(isModifyActive(event)) {
                if (GlobalProjectManager::instance()->isAnnoSelected(shape->relatedAnno())) {
                    qreal deltaX = event->pos().x() - event->lastPos().x();
                    qreal deltaY = event->pos().y() - event->lastPos().y();
                    shape->shapeMoveBy(deltaX, deltaY);
                }
            } else if(!isModifyEvent(event)) {
                if (shape->parentImage() != NULL) {
                    shape->parentImage()->exMouseMoveEvent(event);
                }
            }
        }

        void ToolBrush::mousePressEvent(AnnoGraphicsPixmap *img,
                                        QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if(_modify) {
                resetModify();
            } else if(!isModifyEvent(event)) {
                if (_curShape == NULL) {
//					GlobalLogger::instance()->logDebug("_curShape == NULL");
                    dt::AnnoBrush *apoly = new dt::AnnoBrush();

                    apoly->append(img->mapFromScene(event->scenePos()));
                    QUuid parentId = GlobalToolManager::instance()->getLockedAnno();
                    dt::Annotation *anno = new dt::Annotation();
                    anno->setAnnoId(QUuid::createUuid());
                    anno->setShape(apoly);
                    if (!parentId.isNull()) {
                        _curParentAnno = GlobalProjectManager::instance()->selectedFile()->getAnnotation(parentId);
                        anno->setAnnoParent(parentId);
                    }

                    AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(anno);
                    if (s != NULL) {
                        _scene->addAnnoShape(s);
                        _curShape = static_cast<AnnoGraphicsBrush *>(s);
                        _curShape->setClosedDrawing(false);
                        _scene->selectShape(anno->annoId());
                    }
                } else {
//					GlobalLogger::instance()->logDebug("_curPolygon !!!= NULL");
                    _curShape->appendPolygonPoint(img->mapFromScene(event->scenePos()));
                }
            }
        }

        void ToolBrush::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneMouseEvent *event) {
            if(event->button() == Qt::RightButton) {
                //TODO think of right click handling during active poly mode
                if(_curShape == NULL) {
                    switchDefaultTool();
                } else {
                    if (!_modify && _curShape != NULL && _scene != NULL) {
                        escape();
                    }
                }
                return;
            } else if(event->button() != Qt::LeftButton) {
                return;
            }

            if (isModifyActive(event)) {

            } else if(!isModifyEvent(event)) {
                if (_curShape != NULL) {
                    _curShape->cpMouseReleaseEvent(_curShape->cpCount() - 1, event);
                }
            }
        }

        void ToolBrush::mouseMoveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneMouseEvent *event) {
//			GlobalLogger::instance()->logDebug("ToolPolygon::mouseMoveEvent");

            if (isModifyActive(event)) {

            } else if(!isModifyEvent(event)) {
                if (_curShape != NULL) {
//					GlobalLogger::instance()->logDebug("curPolygon != NULL");
//					GlobalLogger::instance()->logDebug(QString("%1").arg(_curShape->cpCount() - 1));
                    _curShape->cpMouseMoveEvent(_curShape->cpCount() - 1, event);
                }
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
                _curShape = (AnnoGraphicsBrush *) shape;
            }
        }

        void ToolBrush::hoverLeaveEvent(AnnoGraphicsShape *shape,
                                        QGraphicsSceneHoverEvent *event) {

        }

        void ToolBrush::hoverEnterEvent(AnnoGraphicsControlPoint *cp,
                                        QGraphicsSceneHoverEvent *event) {

        }

        void ToolBrush::hoverLeaveEvent(AnnoGraphicsControlPoint *cp,
                                        QGraphicsSceneHoverEvent *event) {

        }

        void ToolBrush::keyReleaseEvent(AnnoGraphicsControlPoint *cp, QKeyEvent *event) {
            handleEscape(event);
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
