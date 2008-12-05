#ifndef AFRXOR_H_
#define AFRXOR_H_

#include "LogicFilterRule.h"

namespace anno {
    namespace filter {

        class AfrXor: public LogicFilterRule {
            private:
                static const QString XML_NAME;

            public:
                AfrXor(bool autoDelete = true);
                virtual ~AfrXor();

                // Class specific methods
            public:
                static bool isXmlName(const QString &str);
                static QString xmlName();
                static AfrXor *fromXml(QXmlStreamReader &reader) throw(exc::XmlException *);

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
                // end: inherited interface stuff

        };

        // Inlining
        // ------------------------------------------------------------------------------------
        inline bool AfrXor::isXmlName(const QString &str) {
            return (QString::compare(str, XML_NAME, Qt::CaseInsensitive) == 0);
        }

        inline QString AfrXor::xmlName() {
            return XML_NAME;
        }
        // ------------------------------------------------------------------------------------


    }
}

#endif /* AFRXOR_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
