#ifndef ANNOGRAPHICSSHAPE_H_
#define ANNOGRAPHICSSHAPE_H_

#include "Annotation.h"
#include "AnnoGraphicsControlPoint.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QList>

namespace anno {
    namespace graphics {
        class AnnoGraphicsPixmap;

        class AnnoGraphicsShape {
                // Members that are managed for child classes
            protected:
                AnnoGraphicsPixmap *_parentImage;
                dt::Annotation *_anno;
                QList<AnnoGraphicsControlPoint *> _cp;

                // Constructors
            protected:
                AnnoGraphicsShape(dt::Annotation *anno, AnnoGraphicsPixmap *img = 0);

                // Destructor
            public:
                virtual ~AnnoGraphicsShape();

                // General access methods
            public:
                dt::Annotation *relatedAnno();
                AnnoGraphicsPixmap *parentImage();
                void setParentImage(AnnoGraphicsPixmap *img);

                // general interface including tool support
            public:
                virtual QGraphicsItem *graphicsItem() = 0;
                virtual void shapeMoveBy(qreal deltaX, qreal deltaY) = 0;
                virtual void shapeSizeBy(qreal facX, qreal facY) = 0;

                // general CP interface
            public:
                virtual void initControlPoints();
                virtual void validateCpIndices();
                virtual void insertControlPoint(int index, AnnoGraphicsControlPoint *cp);
                virtual void removeControlPoint(int index);
                virtual void moveControlPoint(int index, qreal x, qreal y);
                virtual void moveControlPointTo(int index, qreal x, qreal y);
                virtual void setControlPointsVisible(bool vis);
                virtual int cpCount() const;

                // Mouse interaction interface for CPs
            public:
                virtual void cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event);
                virtual void cpMouseReleaseEvent(int index, QGraphicsSceneMouseEvent *event);
                virtual void cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event);

                // external mouse interface
            public:
                virtual void exMouseMoveEvent(QGraphicsSceneMouseEvent *event);
                virtual void exMousePressEvent(QGraphicsSceneMouseEvent *event);
                virtual void exMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        };

    }
}

#endif /*ANNOGRAPHICSSHAPE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
