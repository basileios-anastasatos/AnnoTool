#ifndef TOOLPOLYGON_H_
#define TOOLPOLYGON_H_

#include "GraphicsTool.h"
#include "AnnoGraphicsPolygon.h"

#include "GlobalProjectManager.h"

namespace anno {
    namespace graphics {
        class AnnoGraphicsShape;

        class ToolPolygon : public GraphicsTool {
            private:
                QCursor _cursorNormal;
                QCursor _cursorActive;
                AnnoGraphicsPolygon *_curPolygon;
                bool _modify;

            private:
                bool isModifyEvent(QGraphicsSceneMouseEvent *event);
                bool isModifyEvent(QGraphicsSceneHoverEvent *event);
                bool isModifyResetEvent(QGraphicsSceneMouseEvent *event);
                bool isModifyResetEvent(QGraphicsSceneHoverEvent *event);
                bool isModifyActive(QGraphicsSceneMouseEvent *event);
                bool isModifyActive(QGraphicsSceneHoverEvent *event);
                bool isType(AnnoGraphicsShape *shape);
                void resetModify();
                void setModify(AnnoGraphicsShape *shape);

            public:
                ToolPolygon(QGraphicsView *view, AnnoGraphicsScene *scene);
                virtual ~ToolPolygon();

                // general tool information
            public:
                virtual bool handlesCp() const;
                virtual bool handlesShape() const;
                virtual bool handlesImage() const;

                // mouse interface
            public:
                virtual void mousePressEvent(AnnoGraphicsControlPoint *cp,
                                             QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(AnnoGraphicsControlPoint *cp,
                                               QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(AnnoGraphicsControlPoint *cp,
                                            QGraphicsSceneMouseEvent *event);

                virtual void mousePressEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(AnnoGraphicsShape *shape,
                                               QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(AnnoGraphicsShape *shape,
                                            QGraphicsSceneMouseEvent *event);

                virtual void mousePressEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneMouseEvent *event);
                virtual void mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                               QGraphicsSceneMouseEvent *event);
                virtual void mouseMoveEvent(AnnoGraphicsPixmap *img,
                                            QGraphicsSceneMouseEvent *event);

                virtual void hoverEnterEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsPixmap *img,
                                             QGraphicsSceneHoverEvent *event);

                virtual void hoverEnterEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsShape *shape,
                                             QGraphicsSceneHoverEvent *event);

                virtual void hoverEnterEvent(AnnoGraphicsControlPoint *cp,
                                             QGraphicsSceneHoverEvent *event);
                virtual void hoverLeaveEvent(AnnoGraphicsControlPoint *cp,
                                             QGraphicsSceneHoverEvent *event);

        };

        // inlining
        // ---------------------------------------------------------
        inline bool ToolPolygon::isModifyEvent(QGraphicsSceneMouseEvent *event) {
            return ((event->modifiers() & Qt::ControlModifier) != 0);
        }

        inline bool ToolPolygon::isModifyEvent(QGraphicsSceneHoverEvent *event) {
            return ((event->modifiers() & Qt::ControlModifier) != 0);
        }

        inline bool ToolPolygon::isModifyResetEvent(QGraphicsSceneMouseEvent *event) {
            return (_modify && !isModifyEvent(event));
        }

        inline bool ToolPolygon::isModifyResetEvent(QGraphicsSceneHoverEvent *event) {
            return (_modify && !isModifyEvent(event));
        }

        inline bool ToolPolygon::isModifyActive(QGraphicsSceneMouseEvent *event) {
            return (_modify && isModifyEvent(event));
        }

        inline bool ToolPolygon::isModifyActive(QGraphicsSceneHoverEvent *event) {
            return (_modify && isModifyEvent(event));
        }

        inline bool ToolPolygon::isType(AnnoGraphicsShape *shape) {
            return (shape != NULL && shape->shapeType() == dt::ASTypePolygon);
        }

        inline void ToolPolygon::resetModify() {
            _modify = false;
            anno::GlobalProjectManager::instance()->setSelectedAnnoRow(-1);
            _curPolygon = NULL;
        }

        inline void ToolPolygon::setModify(AnnoGraphicsShape *shape) {
            anno::GlobalProjectManager::instance()->setSelectedAnnoRow(shape->relatedAnno()->annoId());
            _curPolygon = (AnnoGraphicsPolygon *) shape;
            _modify = true;
        }
        // ---------------------------------------------------------

    }
}

#endif /*TOOLPOLYGON_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
