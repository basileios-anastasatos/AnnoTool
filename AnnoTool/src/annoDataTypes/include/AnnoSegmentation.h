#pragma once

#include <QPolygonF>
#include "AnnoShape.h"

namespace anno {
    namespace dt {

        class AnnoSegmenation : public QPolygonF, public AnnoShape {
            public:
                AnnoSegmenation();
                virtual ~AnnoSegmenation();

            public:
                virtual AnnoShapeType shapeType() const;
                virtual QString shapeInfo() const;
                virtual QRectF boundingRect() const;

            public:
                virtual void print() const;

            public:
                AnnoSegmenation &operator=(const QPolygonF &r);
                AnnoSegmenation &operator=(const AnnoSegmenation &r);

            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
