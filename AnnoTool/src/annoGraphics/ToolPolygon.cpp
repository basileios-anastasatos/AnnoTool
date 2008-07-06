#include "include/ToolPolygon.h"
#include <QGraphicsSceneMouseEvent>
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
            GraphicsTool(view, scene), _curPolygon(NULL) {
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
            if (_curPolygon != NULL && cp->getIndex() == 0) {
                //event->accept();
                _curPolygon->setClosedDrawing(true);
                _curPolygon->update();
                _curPolygon = NULL;
            } else {
                if (cp->parentShape() != NULL) {
                    GlobalLogger::instance()->logDebug("Tool-Poly: CP: mousePressEvent");
                    //cp->parentShape()->exMousePressEvent(event);
                    //TODO This must be bug-fixed!
                }
            }
        }

        void ToolPolygon::mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                            QGraphicsSceneMouseEvent *event) {
            if (cp->parentShape() != NULL) {
                GlobalLogger::instance()->logDebug("Tool-Poly: CP: mouseReleaseEvent");
                //				cp->parentShape()->exMouseReleaseEvent(event);
                //TODO This must be bug-fixed!
            }
        }

        void ToolPolygon::mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                         QGraphicsSceneMouseEvent *event) {
            if (cp->parentShape() != NULL) {
                GlobalLogger::instance()->logDebug("Tool-Poly: CP: mouseMoveEvent");
                //				cp->parentShape()->exMouseMoveEvent(event);
                //TODO This must be bug-fixed!
            }
        }

        void ToolPolygon::mousePressEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMousePressEvent(event);
            }
        }

        void ToolPolygon::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                            QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMouseReleaseEvent(event);
            }
        }

        void ToolPolygon::mouseMoveEvent(AnnoGraphicsShape *shape,
                                         QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMouseMoveEvent(event);
            }
        }

        void ToolPolygon::mousePressEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneMouseEvent *event) {
            event->accept();
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

        void ToolPolygon::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                            QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("ToolPolygon::mouseReleaseEvent");
            //event->accept();
            if (_curPolygon != NULL) {
                GlobalLogger::instance()->logDebug("curPolygon != NULL");
                GlobalLogger::instance()->logDebug("hmm..");
                _curPolygon->cpMouseReleaseEvent(_curPolygon->cpCount() - 1, event);
                GlobalLogger::instance()->logDebug("huumm..");
            }
        }

        void ToolPolygon::mouseMoveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("ToolPolygon::mouseMoveEvent");
            //event->accept();
            if (_curPolygon != NULL) {
                GlobalLogger::instance()->logDebug("curPolygon != NULL");
                GlobalLogger::instance()->logDebug(QString("%1").arg(_curPolygon->cpCount() - 1));
                _curPolygon->cpMouseMoveEvent(_curPolygon->cpCount() - 1, event);
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

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
