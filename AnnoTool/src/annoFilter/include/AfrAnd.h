#ifndef AFRAND_H_
#define AFRAND_H_

#include "LogicFilterRule.h"

namespace anno {
    namespace filter {

        class AfrAnd : public LogicFilterRule {
            public:
                AfrAnd(bool autoDelete = true);
                virtual ~AfrAnd();

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
                // end: inherited interface stuff

        };

    }
}

#endif /* AFRAND_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
