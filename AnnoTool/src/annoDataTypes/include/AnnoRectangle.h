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
                virtual QString shapeInfo() const;
                virtual QRectF boundingRect() const;

            public:
                virtual void print() const;

            public:
                AnnoRectangle &operator=(const QRectF &r);
                AnnoRectangle &operator=(const AnnoRectangle &r);

            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

    }
}

#endif /*ANNORECTANGLE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
