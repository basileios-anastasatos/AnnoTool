#ifndef ANNORECTANGLE_H_
#define ANNORECTANGLE_H_

#include <QRectF>
#include "AnnoShape.h"

namespace anno {

    namespace dt {

        class AnnoRectangle : public QRectF, public AnnoShape {
            public:
                AnnoRectangle();
                virtual ~AnnoRectangle();

            public:
                virtual AnnoShapeType shapeType() const;

            public:
                virtual void print() const;

            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

    }

}

#endif /*ANNORECTANGLE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
