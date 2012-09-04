#pragma once

#include "GraphicsTool.h"
#include "GlobalProjectManager.h"
#include "AnnoGraphicsShape.h"

namespace anno {
    namespace dt {
        class Annotation;
        class Segmentation;
    }

    namespace graphics {
        class AnnoGraphicsPath;

        class ToolBrush : public GraphicsTool {
            private:
                QCursor				_cursorNormal;
                QCursor				_cursorActive;
                AnnoGraphicsPath 	*_curShape;
                dt::Annotation		*_curParentAnno;
                bool				_modify;
                bool				_bFGBrush;
                bool				_bDrag;

            private:
                void escape();
                void handleEscape(QKeyEvent *event);
                void switchDefaultTool();

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
                ToolBrush(QGraphicsView *view, AnnoGraphicsScene *scene);
                virtual ~ToolBrush();

                // general tool information
            public:
                virtual bool handlesCp() const;
                virtual bool handlesShape() const;
                virtual bool handlesImage() const;

                // general tool interface
            public:
                //virtual void toolActivate();
                virtual void toolDeactivate();

                // mouse interface
            public:
                virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

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

                virtual void keyReleaseEvent(AnnoGraphicsShape *shape, QKeyEvent *event);
                virtual void keyReleaseEvent(AnnoGraphicsPixmap *img, QKeyEvent *event);

        };

        // inlining
        // ---------------------------------------------------------
        inline bool ToolBrush::isModifyEvent(QGraphicsSceneMouseEvent *event) {
            return ((event->modifiers() & Qt::ControlModifier) != 0);
        }

        inline bool ToolBrush::isModifyEvent(QGraphicsSceneHoverEvent *event) {
            return ((event->modifiers() & Qt::ControlModifier) != 0);
        }

        inline bool ToolBrush::isModifyResetEvent(QGraphicsSceneMouseEvent *event) {
            return (_modify && !isModifyEvent(event));
        }

        inline bool ToolBrush::isModifyResetEvent(QGraphicsSceneHoverEvent *event) {
            return (_modify && !isModifyEvent(event));
        }

        inline bool ToolBrush::isModifyActive(QGraphicsSceneMouseEvent *event) {
            return (_modify && isModifyEvent(event));
        }

        inline bool ToolBrush::isModifyActive(QGraphicsSceneHoverEvent *event) {
            return (_modify && isModifyEvent(event));
        }

        inline bool ToolBrush::isType(AnnoGraphicsShape *shape) {
            return (shape != NULL && shape->shapeType() == dt::ASTypePath);
        }

        inline void ToolBrush::resetModify() {
            _modify = false;
            anno::GlobalProjectManager::instance()->setSelectedAnnoRow(-1);
            _curShape = NULL;
        }

        inline void ToolBrush::setModify(AnnoGraphicsShape *shape) {
            anno::GlobalProjectManager::instance()->setSelectedAnnoRow(shape->relatedAnno()->annoId());
            _curShape = (AnnoGraphicsPath *) shape;
            _modify = true;
        }
        // ---------------------------------------------------------

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
