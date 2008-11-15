#ifndef AFRNOT_H_
#define AFRNOT_H_

#include "LogicFilterRule.h"

namespace anno {
    namespace filter {

        class AfrNot: public LogicFilterRule {
            public:
                AfrNot(bool autoDelete = true);
                AfrNot(AnnoFilterRule *rule, bool autoDelete = true);
                virtual ~AfrNot();

                // inherited interface stuff
                // ------------------------------------------------------------------------------------
                // General interface
            public:
                virtual bool isValid() const;
                virtual QString toString() const;

                // Child-Access interface
            public:
                virtual void addChild(AnnoFilterRule *child);

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

                // <not>-specific stuff
            public:
                bool hasOperand() const;
                void setOperand(AnnoFilterRule *rule);
                void setOperandClean(AnnoFilterRule *rule);
                AnnoFilterRule *getOperand();
                const AnnoFilterRule *getOperand() const;
        };

    }
}

#endif /* AFRNOT_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
