#ifndef ANNOSINGLEPOINT_H_
#define ANNOSINGLEPOINT_H_

#include <QPointF>
#include "AnnoShape.h"

namespace anno {

    namespace dt {

        class AnnoSinglePoint : public QPointF, public AnnoShape {
            public:
                AnnoSinglePoint();
                virtual ~AnnoSinglePoint();

            public:
                virtual AnnoShapeType shapeType() const;

            public:
                virtual void print() const;

            public:
                AnnoSinglePoint &operator=(const QPointF &point);
                AnnoSinglePoint &operator=(const AnnoSinglePoint &point);

            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

    }

}

#endif /*ANNOSINGLEPOINT_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
