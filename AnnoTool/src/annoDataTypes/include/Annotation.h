#ifndef ANNOTATION_H_
#define ANNOTATION_H_

#include <QUuid>
#include <QList>
#include "AllAnnoExceptions.h"
#include "AnnoShape.h"

class QXmlStreamWriter;
class QXmlStreamReader;

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;

        class AnnoAttribute {
            public:
                QString name;
                QString className;
                QString value;

            private:
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);

            public:
                AnnoAttribute();
                AnnoAttribute(const QString &name, const QString &className,
                              const QString &value);
                virtual ~AnnoAttribute();

            public:
                void print() const;

            public:
                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoAttribute fromXml(QXmlStreamReader &reader)
                throw(XmlException *);
        };

        class Annotation {
            private:
                QUuid _annoId;
                QString _comment;
                QList<QString> _annoClasses;
                QList<AnnoAttribute> _annoAttributes;
                AnnoShape *_shape;

            private:
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
                void annoClassesToXml(QXmlStreamWriter &writer) const
                throw(XmlException *);
                void annoAttributesToXml(QXmlStreamWriter &writer) const
                throw(XmlException *);
                void loadAnnoClassesFromXml(QXmlStreamReader &reader)
                throw(XmlException *);
                void loadAnnoAttributesFromXml(QXmlStreamReader &reader)
                throw(XmlException *);

            public:
                Annotation();
                virtual ~Annotation();

            public:
                QUuid annoId() const;
                QString annoIdAsString() const;
                QString comment() const;
                AnnoShape *shape();
                QList<QString> *classes();
                QList<AnnoAttribute> *attributes();

            public:
                void setAnnoId(const QUuid &uuid);
                void setComment(const QString &comment);
                void setShape(AnnoShape *shape);

            public:
                void print() const;

            public:
                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static Annotation *fromXml(QXmlStreamReader &reader)
                throw(XmlException *);
        };

    } //end namespace dt
} //end namespace anno

#endif /*ANNOTATION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
