#include "include/AfrXor.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace anno {
    namespace filter {

        AfrXor::AfrXor(bool autoDelete) :
            LogicFilterRule(autoDelete) {
            // nothing to do here
        }

        AfrXor::~AfrXor() {
            printf("delete <xor>\n");
        }

        bool AfrXor::isValid() const {
            if (_rules.size() < 2) {
                return false;
            }
            return LogicFilterRule::isValid();
        }

        QString AfrXor::toString() const {
            QString str("(xor");
            str.append(LogicFilterRule::toString());
            str.append(")");
            return str;
        }

        void AfrXor::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeStartElement("xor");
            LogicFilterRule::toXml(writer);
            writer.writeEndElement();
        }

        void AfrXor::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            //TODO implement this
        }

        bool AfrXor::eval(const dt::Annotation *anno) const throw(exc::IllegalStateException *) {
            if (_rules.size() < 2) {
                throw new exc::IllegalStateException(__FILE__, __LINE__,
                                                     "<xor>-rule has too few operands.");
            }

            bool xorValue = _rules[0]->eval(anno);
            int size = _rules.size();
            for(int i = 1; i < size; ++i) {
                bool tmp = _rules[i]->eval(anno);
                xorValue = ((xorValue && !tmp) || (!xorValue && tmp));
            }
            return xorValue;
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
