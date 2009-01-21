#ifndef AFROR_H_
#define AFROR_H_

#include "LogicFilterRule.h"

namespace anno {
    namespace filter {

        class AfrOr : public LogicFilterRule {
            private:
                static const QString XML_NAME;

            public:
                AfrOr(bool autoDelete = true);
                virtual ~AfrOr();

                // Class specific methods
            public:
                static bool isXmlName(const QString &str);
                static QString xmlName();
                static AfrOr *fromXml(QXmlStreamReader &reader) throw(exc::XmlException *);

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
                // end: inherited interface stuff

        };

        // Inlining
        // ------------------------------------------------------------------------------------
        inline bool AfrOr::isXmlName(const QString &str) {
            return (QString::compare(str, XML_NAME, Qt::CaseInsensitive) == 0);
        }

        inline QString AfrOr::xmlName() {
            return XML_NAME;
        }
        // ------------------------------------------------------------------------------------


    }
}

#endif /* AFROR_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
