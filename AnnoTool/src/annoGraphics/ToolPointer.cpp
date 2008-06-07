#include "include/ToolPointer.h"
#include "AllAnnoGraphics.h"
#include <QGraphicsSceneMouseEvent>

#include "importGlobals.h"

namespace anno {
    namespace graphics {

        ToolPointer::ToolPointer(AnnoGraphicsScene *scene) :
            GraphicsTool(scene) {
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
            event->accept();
            cp->parentShape()->cpMousePressEvent(cp->getIndex(), event);
        }

        void ToolPointer::mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                            QGraphicsSceneMouseEvent *event) {
            event->accept();
            cp->parentShape()->cpMouseReleaseEvent(cp->getIndex(), event);
        }

        void ToolPointer::mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                         QGraphicsSceneMouseEvent *event) {
            event->accept();
            cp->parentShape()->cpMouseMoveEvent(cp->getIndex(), event);
        }

        void ToolPointer::mousePressEvent(AnnoGraphicsShape *shape,
                                          QGraphicsSceneMouseEvent *event) {
            event->accept();
            //_scene->selectShape(shape->relatedAnno()->annoId());
            GlobalProjectManager::instance()->setSelectedAnnoRow(shape->relatedAnno()->annoId());
            //TODO so anpassen dass an dieser stelle eine UUID selektiert werden kann!
        }

        void ToolPointer::mouseReleaseEvent(AnnoGraphicsShape *shape,
                                            QGraphicsSceneMouseEvent *event) {
            event->accept();
        }

        void ToolPointer::mouseMoveEvent(AnnoGraphicsShape *shape,
                                         QGraphicsSceneMouseEvent *event) {
            event->accept();
            qreal deltaX = event->pos().x() - event->lastPos().x();
            qreal deltaY = event->pos().y() - event->lastPos().y();
            shape->shapeMoveBy(deltaX, deltaY);
        }

        void ToolPointer::mousePressEvent(AnnoGraphicsPixmap *shape,
                                          QGraphicsSceneMouseEvent *event) {
            event->accept();
            _scene->clearSelection();
        }

    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
