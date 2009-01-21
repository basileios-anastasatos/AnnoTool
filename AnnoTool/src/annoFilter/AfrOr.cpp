#include "include/AfrOr.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;

namespace anno {
    namespace filter {
        const QString AfrOr::XML_NAME("or");

        AfrOr::AfrOr(bool autoDelete) :
            LogicFilterRule(autoDelete) {
            // nothing to do here
        }

        AfrOr::~AfrOr() {
        }

        AfrOr *AfrOr::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            AfrOr *pRule = new AfrOr(true);

            try {
                pRule->loadFromXml(reader);
            } catch(exc::XmlException *e) {
                delete pRule;
                throw e;
            }

            return pRule;
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
            writer.writeStartElement(XML_NAME);
            LogicFilterRule::toXmlInternal(writer);
            writer.writeEndElement();
        }

        void AfrOr::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || !isXmlName(curParent)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_NAME, curParent);
            }

            XmlHelper::skipToNextStartElement(true, reader);
            LogicFilterRule::loadFromXmlInternal(XML_NAME, reader);
        }

        bool AfrOr::evalInternal(const dt::Annotation *anno) const
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
