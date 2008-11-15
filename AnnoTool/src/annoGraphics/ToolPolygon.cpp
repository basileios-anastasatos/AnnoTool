#include "include/ToolPolygon.h"
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsPolygon.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "AnnoToolMainWindow.h"


namespace anno {
    namespace graphics {

        ToolPolygon::ToolPolygon(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene), _curShape(NULL), _curParentAnno(NULL), _modify(false) {
            _cursorNormal = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curPoly")), 0, 0);
            _cursorActive = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curPoly_active")), 0, 0);
        }

        ToolPolygon::~ToolPolygon() {
        }

        bool ToolPolygon::handlesCp() const {
            return false;
        }

        bool ToolPolygon::handlesShape() const {
            return false;
        }

        bool ToolPolygon::handlesImage() const {
            return true;
        }

        void ToolPolygon::escape() {
            _scene->removeAnnoShape(_curShape);
            _curShape = NULL;
            _curParentAnno = NULL;
        }

        void ToolPolygon::handleEscape(QKeyEvent *event) {
            if (!_modify && event->key() == Qt::Key_Escape && _curShape != NULL && _scene != NULL) {
                escape();
            }
        }

        void ToolPolygon::finishPolygon() {
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

        void ToolPolygon::switchDefaultTool() {
            if (!_modify && _curShape != NULL && _scene != NULL) {
                escape();
            }
            GlobalToolManager::instance()->selectToolDefault();
        }

        void ToolPolygon::toolDeactivate() {
            if (_curShape != NULL) {
                escape();
            }
        }

        void ToolPolygon::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (!_modify && _curShape != NULL) {
                _curShape->removePolygonPoint(_curShape->polygon().size() - 1);
                finishPolygon();
            }
        }

        void ToolPolygon::mousePressEvent(AnnoGraphicsControlPoint *cp,
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

        void ToolPolygon::mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                            QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if(isModifyActive(event)) {
                cp->parentShape()->cpMouseReleaseEvent(cp->getIndex(), event);
            }
        }

        void ToolPolygon::mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                         QGraphicsSceneMouseEvent *event) {
            if(isModifyActive(event)) {
                cp->parentShape()->cpMouseMoveEvent(cp->getIndex(), event);
            }
        }

        void ToolPolygon::mousePressEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if(isModifyResetEvent(event)) {
                resetModify();
            }

            if(isModifyActive(event)) {
                if (isType(shape)) {
                    AnnoGraphicsPolygon *poly = (AnnoGraphicsPolygon *)shape;
                    poly->insertPolygonPoint(event->pos());
                }
            } else if(!isModifyEvent(event)) {
                if (shape->parentImage() != NULL) {
                    shape->parentImage()->exMousePressEvent(event);
                }
            }
        }

        void ToolPolygon::mouseReleaseEvent(AnnoGraphicsShape *shape,
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

        void ToolPolygon::mouseMoveEvent(AnnoGraphicsShape *shape,
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

        void ToolPolygon::mousePressEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if(_modify) {
                resetModify();
            } else if(!isModifyEvent(event)) {
                if (_curShape == NULL) {
//					GlobalLogger::instance()->logDebug("_curShape == NULL");
                    dt::AnnoPolygon *apoly = new dt::AnnoPolygon();

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
                        _curShape = static_cast<AnnoGraphicsPolygon *>(s);
                        _curShape->setClosedDrawing(false);
                        _scene->selectShape(anno->annoId());
                    }
                } else {
//					GlobalLogger::instance()->logDebug("_curPolygon !!!= NULL");
                    _curShape->appendPolygonPoint(img->mapFromScene(event->scenePos()));
                }
            }
        }

        void ToolPolygon::mouseReleaseEvent(AnnoGraphicsPixmap *img,
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

        void ToolPolygon::mouseMoveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneMouseEvent *event) {
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

        void ToolPolygon::hoverEnterEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_cursorNormal);
            }
        }

        void ToolPolygon::hoverLeaveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(Qt::ArrowCursor);
            }
        }

        void ToolPolygon::hoverEnterEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneHoverEvent *event) {
            if (_curShape == NULL && isModifyEvent(event) && isType(shape)) {
                _curShape = (AnnoGraphicsPolygon *) shape;
            }
        }

        void ToolPolygon::hoverLeaveEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneHoverEvent *event) {

        }

        void ToolPolygon::hoverEnterEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneHoverEvent *event) {

        }

        void ToolPolygon::hoverLeaveEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneHoverEvent *event) {

        }

        void ToolPolygon::keyReleaseEvent(AnnoGraphicsControlPoint *cp, QKeyEvent *event) {
            handleEscape(event);
        }

        void ToolPolygon::keyReleaseEvent(AnnoGraphicsShape *shape, QKeyEvent *event) {
            handleEscape(event);
        }

        void ToolPolygon::keyReleaseEvent(AnnoGraphicsPixmap *img, QKeyEvent *event) {
            handleEscape(event);
        }


    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
