#include "include/AfrAnd.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace anno {

    namespace filter {

        AfrAnd::AfrAnd(bool autoDelete) : LogicFilterRule(autoDelete) {
            // nothing to do here
        }

        AfrAnd::~AfrAnd() {
            printf("delete <and>\n");
        }

        bool AfrAnd::isValid() const {
            if(_rules.size() < 2) {
                return false;
            }
            return LogicFilterRule::isValid();
        }

        QString AfrAnd::toString() const {
            QString str("(and");
            str.append(LogicFilterRule::toString());
            str.append(")");
            return str;
        }

        void AfrAnd::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeStartElement("and");
            LogicFilterRule::toXml(writer);
            writer.writeEndElement();
        }

        void AfrAnd::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            //TODO implement this
        }

        bool AfrAnd::eval(const dt::Annotation *anno) const throw(exc::IllegalStateException *) {
            if(_rules.size() < 2) {
                throw new exc::IllegalStateException(__FILE__, __LINE__, "<and>-rule has too few operands.");
            }

            foreach(AnnoFilterRule * r, _rules) {
                if(!r->eval(anno)) {
                    return false;
                }
            }
            return true;
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
