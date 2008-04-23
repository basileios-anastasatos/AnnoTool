#ifndef ANNOCLASSDEFINITION_H_
#define ANNOCLASSDEFINITION_H_

#include <QString>
#include <QList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AllAnnoExceptions.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;
        class AnnoClassDefinition {
                // private class members
                //-------------------------------------------
            private:
                QString _className;
                QList<QString> _classAttributes;
                AnnoClassDefinition *_parent;
                //-------------------------------------------

                // public constructors/destructor
                //-------------------------------------------
            public:
                AnnoClassDefinition();
                AnnoClassDefinition(const QString &className);
                AnnoClassDefinition(const QString &className,
                                    AnnoClassDefinition *parent);
                virtual ~AnnoClassDefinition();
                //-------------------------------------------

                // public setter/getter
                //-------------------------------------------
            public:
                QString name() const;
                const AnnoClassDefinition *parent() const;
                const QList<QString> *const attributes() const;
                void allAttributes(QList<QString> &lst) const;
                bool hasParent() const;
                void setName(QString name);
                void setParent(AnnoClassDefinition *parent);
                //-------------------------------------------

                // public attributes access
                //-------------------------------------------
            public:
                bool hasAttribute(const QString &attrName) const;
                void addAttribute(const QString &attrName)
                throw(NameConflictException *);
                void removeAttribute(const QString &attrName);
                //-------------------------------------------

            public:
                void print() const;

                // public XML stuff
                //-------------------------------------------
            public:
                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                void attributesFromXml(QXmlStreamReader &reader)
                throw(XmlException *);
                //-------------------------------------------

        };
    } //end namespace dt
} //end namespace anno

#endif /*ANNOCLASSDEFINITION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
