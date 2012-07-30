#pragma once

#include <QPainterPath>
#include "AnnoShape.h"

namespace anno {
    namespace dt {

        class AnnoPath : public QPainterPath, public AnnoShape {
            public:
                AnnoPath();
                virtual ~AnnoPath();

            public:
                virtual AnnoShapeType shapeType() const;
                virtual QString shapeInfo() const;
                virtual QRectF boundingRect() const;
                virtual QPainterPath path() const;

            public:
                virtual void print() const;

            public:
                AnnoPath &operator=(const QPainterPath &r);
                AnnoPath &operator=(const AnnoPath &r);

            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
