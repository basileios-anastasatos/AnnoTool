#ifndef ANNOGRAPHICSSHAPE_H_
#define ANNOGRAPHICSSHAPE_H_

#include "Annotation.h"
#include "AnnoGraphicsControlPoint.h"
#include <QGraphicsItem>
#include <QList>

namespace anno {
    namespace graphics {

        class AnnoGraphicsShape {
            protected:
                dt::Annotation *_anno;
                QList<AnnoGraphicsControlPoint *> _cp;

            protected:
                AnnoGraphicsShape(dt::Annotation *anno);

            public:
                virtual ~AnnoGraphicsShape();

            public:
                dt::Annotation *relatedAnno();

            public:
                virtual QGraphicsItem *graphicsItem() = 0;
                virtual void initControlPoints();
                virtual void validateCpIndices();
                virtual void
                insertControlPoint(int index, AnnoGraphicsControlPoint *cp);
                virtual void removeControlPoint(int index);
                virtual void moveControlPoint(int index, qreal x, qreal y);
                virtual void moveControlPointTo(int index, qreal x, qreal y);

        };

    }
}

#endif /*ANNOGRAPHICSSHAPE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
