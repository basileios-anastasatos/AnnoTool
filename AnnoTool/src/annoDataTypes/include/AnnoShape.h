#ifndef ANNOSHAPE_H_
#define ANNOSHAPE_H_

#include "AllAnnoExceptions.h"

class QXmlStreamReader;
class QXmlStreamWriter;
class QGraphicsItem;

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;

        enum AnnoShapeType {
            ASTypeInvalid, ASTypeSinglePoint, ASTypeLine, ASTypeRectangle, ASTypePolygon, ASTypeEllipse
        };

        class AnnoShape {
            protected:
                AnnoShape();

            public:
                virtual ~AnnoShape();

            public:
                virtual AnnoShapeType shapeType() const = 0;
                virtual QString shapeInfo() const = 0;

            public:
                virtual void print() const = 0;

            public:
                virtual void
                toXml(QXmlStreamWriter &writer) const throw(XmlException *) = 0;
                virtual void
                loadFromXml(QXmlStreamReader &reader) throw(XmlException *) = 0;

                static AnnoShape *fromXml(QXmlStreamReader &reader)
                throw(XmlException *);
        };

    } //end namespace dt
} //end namespace anno

#endif /*ANNOSHAPE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
