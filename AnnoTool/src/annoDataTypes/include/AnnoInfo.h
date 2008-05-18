#ifndef ANNOINFO_H_
#define ANNOINFO_H_

#include <QUuid>
#include "AllAnnoExceptions.h"

class QXmlStreamReader;
class QXmlStreamWriter;

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;
        class AnnoInfo {
            private:
                QUuid _annoComplex;

            private:
                void loadFromXml(QXmlStreamReader &reader) throw(XmlFormatException *);

            public:
                AnnoInfo();
                AnnoInfo(const QUuid &uuid);
                virtual ~AnnoInfo();

            public:
                void setComplexId(const QUuid &uuid);
                QUuid annoComplex() const;

            public:
                void print() const;

            public:
                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoInfo fromXml(QXmlStreamReader &reader)
                throw(XmlFormatException *);
        };

    } //end namespace dt
} //end namespace anno

#endif /*ANNOINFO_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
