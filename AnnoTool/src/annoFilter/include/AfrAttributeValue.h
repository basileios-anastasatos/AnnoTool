#ifndef AFRATTRIBUTEVALUE_H_
#define AFRATTRIBUTEVALUE_H_

#include "AnnoFilterRuleAtom.h"
#include "Annotation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace anno {
    namespace filter {

        class AfrAttributeValue: public AnnoFilterRuleAtom {
            private:
                QString _attrClass;
                QString _attrName;
                QString _attrValue;

            public:
                AfrAttributeValue(const QString &attrName, const QString &attrValue = QString());
                AfrAttributeValue(const QString &attrClass, const QString &attrName, const QString &attrValue);
                virtual ~AfrAttributeValue();

                // inherited interface stuff
                // ------------------------------------------------------------------------------------
                // General interface
            public:
                virtual bool isValid() const;
                virtual QString toString() const;

                // XML interface
            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *);

                // Filtering interface
            public:
                virtual bool eval(const dt::Annotation *anno) const
                throw(exc::IllegalStateException *);
                // ------------------------------------------------------------------------------------

        };

    }
}

#endif /* AFRATTRIBUTEVALUE_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
