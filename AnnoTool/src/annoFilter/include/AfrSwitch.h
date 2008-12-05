#ifndef AFRSWITCH_H_
#define AFRSWITCH_H_

#include "AnnoFilterRuleAtom.h"

namespace anno {
    namespace filter {

        class AfrSwitch: public AnnoFilterRuleAtom {
            private:
                static const QString XML_NAME;

            private:
                bool _state;

            private:
                AfrSwitch();

            public:
                AfrSwitch(bool state);
                virtual ~AfrSwitch();

                // Class specific methods
            public:
                static bool isXmlName(const QString &str);
                static QString xmlName();
                static AfrSwitch *fromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
                bool getState() const;

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

        // Inlining
        // ------------------------------------------------------------------------------------
        inline bool AfrSwitch::isXmlName(const QString &str) {
            return (QString::compare(str, XML_NAME, Qt::CaseInsensitive) == 0);
        }

        inline QString AfrSwitch::xmlName() {
            return XML_NAME;
        }

        inline bool AfrSwitch::getState() const {
            return _state;
        }
        // ------------------------------------------------------------------------------------

    }
}

#endif /* AFRSWITCH_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
