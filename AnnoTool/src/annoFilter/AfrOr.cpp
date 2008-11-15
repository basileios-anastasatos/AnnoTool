#include "include/AfrOr.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace anno {
    namespace filter {

        AfrOr::AfrOr(bool autoDelete) :
            LogicFilterRule(autoDelete) {
            // nothing to do here
        }

        AfrOr::~AfrOr() {
            printf("delete <or>\n");
        }

        bool AfrOr::isValid() const {
            if (_rules.size() < 2) {
                return false;
            }
            return LogicFilterRule::isValid();
        }

        QString AfrOr::toString() const {
            QString str("(or");
            str.append(LogicFilterRule::toString());
            str.append(")");
            return str;
        }

        void AfrOr::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeStartElement("or");
            LogicFilterRule::toXml(writer);
            writer.writeEndElement();
        }

        void AfrOr::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            //TODO implement this
        }

        bool AfrOr::eval(const dt::Annotation *anno) const
        throw(exc::IllegalStateException *) {
            if (_rules.size() < 2) {
                throw new exc::IllegalStateException(__FILE__, __LINE__,
                                                     "<or>-rule has too few operands.");
            }

            foreach(AnnoFilterRule * r, _rules) {
                if (r->eval(anno)) {
                    return true;
                }
            }
            return false;
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
