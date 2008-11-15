#ifndef AFRCLASS_H_
#define AFRCLASS_H_

#include "AnnoFilterRuleAtom.h"

namespace anno {
    namespace filter {

        class AfrClass: public AnnoFilterRuleAtom {
            private:
                QString _className;

            public:
                AfrClass(const QString &className);
                virtual ~AfrClass();

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
                virtual bool eval(const dt::Annotation *anno) const throw(exc::IllegalStateException *);
                // ------------------------------------------------------------------------------------

        };

    }
}

#endif /* AFRCLASS_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
