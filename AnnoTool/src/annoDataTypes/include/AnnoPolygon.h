#ifndef ANNOPOLYGON_H_
#define ANNOPOLYGON_H_

#include <QPolygonF>
#include "AnnoShape.h"

namespace anno {
    namespace dt {

        class AnnoPolygon : public QPolygonF, public AnnoShape {
            public:
                AnnoPolygon();
                virtual ~AnnoPolygon();

            public:
                virtual AnnoShapeType shapeType() const;
                virtual QString shapeInfo() const;
                virtual QRectF boundingRect() const;

            public:
                virtual void print() const;

            public:
                AnnoPolygon &operator=(const QPolygonF &r);
                AnnoPolygon &operator=(const AnnoPolygon &r);

            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

    }
}

#endif /*ANNOPOLYGON_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
