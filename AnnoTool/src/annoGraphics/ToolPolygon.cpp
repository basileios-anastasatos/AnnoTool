#include "include/ToolPolygon.h"
#include <QUuid>

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsShapeCreator.h"
#include "importGlobals.h"
#include "AnnoToolMainWindow.h"

namespace anno {
    namespace graphics {

        ToolPolygon::ToolPolygon(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene), _curPolygon(NULL), _modify(false) {
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

        void ToolPolygon::mousePressEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneMouseEvent *event) {
            if(isModifyActive(event)) {
                if((event->modifiers() & Qt::ShiftModifier) != 0) {
                    _curPolygon->removePolygonPoint(cp->getIndex());
                } else {
                    cp->parentShape()->cpMousePressEvent(cp->getIndex(), event);
                }
            } else if(!isModifyEvent(event)) {
                if (_curPolygon != NULL && cp->getIndex() == 0) {
                    _curPolygon->setClosedDrawing(true);
                    _curPolygon->update();
                    _curPolygon = NULL;
                }
            }
        }

        void ToolPolygon::mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                            QGraphicsSceneMouseEvent *event) {
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
            if(isModifyActive(event) || isModifyResetEvent(event)) {
                resetModify();
            } else if(!isModifyEvent(event)) {
                if (_curPolygon == NULL) {
                    GlobalLogger::instance()->logDebug("_curPolygon == NULL");
                    dt::AnnoFileData *annoFile = GlobalProjectManager::instance()->selectedFile();
                    dt::AnnoPolygon *apoly = new dt::AnnoPolygon();

                    apoly->append(img->mapFromScene(event->scenePos()));
                    dt::Annotation *anno = new dt::Annotation();
                    anno->setAnnoId(QUuid::createUuid());
                    anno->setShape(apoly);
                    annoFile->addAnnotation(anno);

                    AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(anno);
                    if (s != NULL) {
                        GlobalLogger::instance()->logDebug("s != NULL");
                        _scene->addAnnoShape(s);
                        _curPolygon = static_cast<AnnoGraphicsPolygon *>(s);
                        _curPolygon->setClosedDrawing(false);
                        GlobalProjectManager::instance()->setSelectedAnnoRow(anno->annoId());
                    }
                } else {
                    GlobalLogger::instance()->logDebug("_curPolygon !!!= NULL");
                    _curPolygon->appendPolygonPoint(img->mapFromScene(event->scenePos()));
                }
                AnnoToolMainWindow::updateUI();
            }
        }

        void ToolPolygon::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                            QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("ToolPolygon::mouseReleaseEvent");

            if (isModifyActive(event)) {

            } else if(!isModifyEvent(event)) {
                if (_curPolygon != NULL) {
                    _curPolygon->cpMouseReleaseEvent(_curPolygon->cpCount() - 1, event);
                }
            }
        }

        void ToolPolygon::mouseMoveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("ToolPolygon::mouseMoveEvent");

            if (isModifyActive(event)) {

            } else if(!isModifyEvent(event)) {
                if (_curPolygon != NULL) {
                    GlobalLogger::instance()->logDebug("curPolygon != NULL");
                    GlobalLogger::instance()->logDebug(QString("%1").arg(_curPolygon->cpCount() - 1));
                    _curPolygon->cpMouseMoveEvent(_curPolygon->cpCount() - 1, event);
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
            if (_curPolygon == NULL && isModifyEvent(event) && isType(shape)) {
                _curPolygon = (AnnoGraphicsPolygon *) shape;
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


    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
