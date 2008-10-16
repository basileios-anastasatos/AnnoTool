#include "include/ToolSinglePoint.h"
#include "importGlobals.h"

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoSinglePoint.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsShapeCreator.h"
#include <QGraphicsView>

namespace anno {
    namespace graphics {

        ToolSinglePoint::ToolSinglePoint(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene) {
            _cursorNormal = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curSPoint")), 0, 0);
            _cursorActive = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curSPoint_active")), 0, 0);
        }

        ToolSinglePoint::~ToolSinglePoint() {
        }

        bool ToolSinglePoint::handlesCp() const {
            return false;
        }

        bool ToolSinglePoint::handlesShape() const {
            return false;
        }

        bool ToolSinglePoint::handlesImage() const {
            return true;
        }

        void ToolSinglePoint::switchDefaultTool() {
            GlobalToolManager::instance()->selectToolDefault();
        }

        void ToolSinglePoint::mousePressEvent(AnnoGraphicsShape *shape,
                                              QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMousePressEvent(event);
            }
        }

        void ToolSinglePoint::mousePressEvent(AnnoGraphicsPixmap *img,
                                              QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorActive);
            }

            dt::AnnoFileData *annoFile = GlobalProjectManager::instance()->selectedFile();
            if (annoFile != NULL) {
                dt::AnnoSinglePoint *asp = new dt::AnnoSinglePoint();
                *asp = img->mapFromScene(event->scenePos());
                dt::Annotation *anno = new dt::Annotation();
                anno->setAnnoId(QUuid::createUuid());
                anno->setShape(asp);
                annoFile->addAnnotation(anno);

                AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(anno);
                if (s != NULL) {
                    _scene->addAnnoShape(s);
                    GlobalProjectManager::instance()->setSelectedAnnoRow(anno->annoId());
                }
                //AnnoToolMainWindow::updateUI();
            }
        }

        void ToolSinglePoint::mouseReleaseEvent(AnnoGraphicsPixmap *img,
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
        }

        void ToolSinglePoint::hoverEnterEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_cursorNormal);
            }
        }

        void ToolSinglePoint::hoverLeaveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(Qt::ArrowCursor);
            }
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
