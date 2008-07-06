#include "include/ToolPointer.h"
#include "AllAnnoGraphics.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

#include "importGlobals.h"

namespace anno {
    namespace graphics {

        ToolPointer::ToolPointer(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene) {
            _cursorNormal = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curPointer")), 0, 0);
            _cursorHilight = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curPointer_hilight")), 0, 0);
            _cursorMoving = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curPointer_moving")), 0, 0);
            _cursorCp = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curPointer_cp")), 0, 0);
            _cursorCpMoving = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curPointer_cp_moving")), 0, 0);
        }

        ToolPointer::~ToolPointer() {
        }

        bool ToolPointer::handlesCp() const {
            return true;
        }

        bool ToolPointer::handlesShape() const {
            return true;
        }

        bool ToolPointer::handlesImage() const {
            return false;
        }

        void ToolPointer::mousePressEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneMouseEvent *event) {
            if(_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorCpMoving);
            }
            cp->parentShape()->cpMousePressEvent(cp->getIndex(), event);
        }

        void ToolPointer::mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                            QGraphicsSceneMouseEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_prevCursors.pop());
            }
            cp->parentShape()->cpMouseReleaseEvent(cp->getIndex(), event);
        }

        void ToolPointer::mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                         QGraphicsSceneMouseEvent *event) {
            cp->parentShape()->cpMouseMoveEvent(cp->getIndex(), event);
        }

        void ToolPointer::mousePressEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            if (GlobalProjectManager::instance()->isAnnoSelected(shape->relatedAnno())) {
                if(_view != NULL) {
                    _prevCursors.push(_view->cursor());
                    _view->setCursor(_cursorMoving);
                }
            }
        }

        void ToolPointer::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                            QGraphicsSceneMouseEvent *event) {
            if (GlobalProjectManager::instance()->isAnnoSelected(shape->relatedAnno())) {
                if (_view != NULL) {
                    _view->setCursor(_prevCursors.pop());
                }
            } else if(shape->graphicsItem()->contains(event->pos())) {
                GlobalProjectManager::instance()->setSelectedAnnoRow(shape->relatedAnno()->annoId());
            }
        }

        void ToolPointer::mouseMoveEvent(AnnoGraphicsShape *shape,
                                         QGraphicsSceneMouseEvent *event) {
            if (GlobalProjectManager::instance()->isAnnoSelected(shape->relatedAnno())) {
                qreal deltaX = event->pos().x() - event->lastPos().x();
                qreal deltaY = event->pos().y() - event->lastPos().y();
                shape->shapeMoveBy(deltaX, deltaY);
            }
        }

        void ToolPointer::mousePressEvent(AnnoGraphicsPixmap *shape,
                                          QGraphicsSceneMouseEvent *event) {
            GlobalProjectManager::instance()->setSelectedAnnoRow(-1);
        }

        void ToolPointer::hoverEnterEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorCp);
            }
        }

        void ToolPointer::hoverLeaveEvent(AnnoGraphicsControlPoint *cp,
                                          QGraphicsSceneHoverEvent *event) {
            if (_view != NULL && !_prevCursors.isEmpty()) {
                _view->setCursor(_prevCursors.pop());
            }
        }

        void ToolPointer::hoverEnterEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorHilight);
            }
        }

        void ToolPointer::hoverLeaveEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneHoverEvent *event) {
            if (_view != NULL && !_prevCursors.isEmpty()) {
                _view->setCursor(_prevCursors.pop());
            }
        }

        void ToolPointer::hoverEnterEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_cursorNormal);
            }
        }

        void ToolPointer::hoverLeaveEvent(AnnoGraphicsPixmap *img,
                                          QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(Qt::ArrowCursor);
            }
        }

    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
