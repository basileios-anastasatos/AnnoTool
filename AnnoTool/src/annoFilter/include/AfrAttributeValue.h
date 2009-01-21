#ifndef AFRATTRIBUTEVALUE_H_
#define AFRATTRIBUTEVALUE_H_

#include "AnnoFilterRuleAtom.h"

namespace anno {
    namespace filter {

        class AfrAttributeValue: public AnnoFilterRuleAtom {
            private:
                static const QString XML_NAME;

            private:
                QString _attrClass;
                QString _attrName;
                QString _attrValue;
                bool _caseSensitive;

            private:
                Qt::CaseSensitivity convCase() const;

            private:
                AfrAttributeValue();

            public:
                AfrAttributeValue(const QString &attrName, bool caseSensitive = true, bool autoDelete = true);
                AfrAttributeValue(const QString &attrName, const QString &attrValue, bool caseSensitive = true, bool autoDelete = true);
                AfrAttributeValue(const QString &attrClass, const QString &attrName, const QString &attrValue, bool caseSensitive = true, bool autoDelete = true);
                virtual ~AfrAttributeValue();

                // Class specific methods
            public:
                static bool isXmlName(const QString &str);
                static QString xmlName();
                static AfrAttributeValue *fromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
                bool isCaseSensitive() const;
                QString getClass() const;
                QString getName() const;
                QString getValue() const;

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
                virtual bool evalInternal(const dt::Annotation *anno) const
                throw(exc::IllegalStateException *);
                // ------------------------------------------------------------------------------------

        };

        // Inlining
        // ------------------------------------------------------------------------------------
        inline bool AfrAttributeValue::isXmlName(const QString &str) {
            return (QString::compare(str, XML_NAME, Qt::CaseInsensitive) == 0);
        }

        inline QString AfrAttributeValue::xmlName() {
            return XML_NAME;
        }

        inline Qt::CaseSensitivity AfrAttributeValue::convCase() const {
            return (_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
        }

        inline bool AfrAttributeValue::isCaseSensitive() const {
            return _caseSensitive;
        }

        inline QString AfrAttributeValue::getClass() const {
            return _attrClass;
        }

        inline QString AfrAttributeValue::getName() const {
            return _attrName;
        }

        inline QString AfrAttributeValue::getValue() const {
            return _attrValue;
        }
        // ------------------------------------------------------------------------------------

    }
}

#endif /* AFRATTRIBUTEVALUE_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
