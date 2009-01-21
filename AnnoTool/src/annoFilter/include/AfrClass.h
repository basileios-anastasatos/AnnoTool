#ifndef AFRCLASS_H_
#define AFRCLASS_H_

#include "AnnoFilterRuleAtom.h"

namespace anno {
    namespace filter {

        class AfrClass: public AnnoFilterRuleAtom {
            private:
                static const QString XML_NAME;

            private:
                QString _className;
                bool _caseSensitive;

            private:
                Qt::CaseSensitivity convCase() const;
                AfrClass();

            public:
                AfrClass(const QString &className, bool caseSensitive = true, bool autoDelete = true);
                virtual ~AfrClass();

                // Class specific methods
            public:
                static bool isXmlName(const QString &str);
                static QString xmlName();
                static AfrClass *fromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
                bool isCaseSensitive() const;
                QString getName() const;

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
                virtual bool evalInternal(const dt::Annotation *anno) const throw(exc::IllegalStateException *);
                // ------------------------------------------------------------------------------------

        };

        // Inlining
        // ------------------------------------------------------------------------------------
        inline bool AfrClass::isXmlName(const QString &str) {
            return (QString::compare(str, XML_NAME, Qt::CaseInsensitive) == 0);
        }

        inline QString AfrClass::xmlName() {
            return XML_NAME;
        }

        inline Qt::CaseSensitivity AfrClass::convCase() const {
            return (_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
        }

        inline bool AfrClass::isCaseSensitive() const {
            return _caseSensitive;
        }

        inline QString AfrClass::getName() const {
            return _className;
        }
        // ------------------------------------------------------------------------------------


    }
}

#endif /* AFRCLASS_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
