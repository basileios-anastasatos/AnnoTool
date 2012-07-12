#pragma once

#include <QPolygonF>
#include "AnnoShape.h"

namespace anno {
    namespace dt {

        class AnnoBrush : public QPolygonF, public AnnoShape {
            public:
                AnnoBrush();
                virtual ~AnnoBrush();

            public:
                virtual AnnoShapeType shapeType() const;
                virtual QString shapeInfo() const;
                virtual QRectF boundingRect() const;

            public:
                virtual void print() const;

            public:
                AnnoBrush &operator=(const QPolygonF &r);
                AnnoBrush &operator=(const AnnoBrush &r);

            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
