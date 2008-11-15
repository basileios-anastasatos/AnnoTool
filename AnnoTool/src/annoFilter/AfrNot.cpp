#include "include/AfrNot.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace anno {
    namespace filter {

        AfrNot::AfrNot(bool autoDelete) :
            LogicFilterRule(autoDelete) {
            // nothing to do here
        }

        AfrNot::AfrNot(AnnoFilterRule *rule, bool autoDelete) :
            LogicFilterRule(autoDelete) {
            _rules.append(rule);
        }

        AfrNot::~AfrNot() {
            printf("delete <not>\n");
        }

        bool AfrNot::isValid() const {
            if (_rules.size() != 1) {
                return false;
            }
            return LogicFilterRule::isValid();
        }

        QString AfrNot::toString() const {
            QString str("(not");
            str.append(LogicFilterRule::toString());
            str.append(")");
            return str;
        }

        void AfrNot::addChild(AnnoFilterRule *child) {
            if(child != NULL && _rules.isEmpty()) {
                LogicFilterRule::addChild(child);
            }
        }

        void AfrNot::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeStartElement("not");
            LogicFilterRule::toXml(writer);
            writer.writeEndElement();
        }

        void AfrNot::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            //TODO implement this
        }

        bool AfrNot::eval(const dt::Annotation *anno) const
        throw(exc::IllegalStateException *) {
            if (_rules.size() != 1) {
                throw new exc::IllegalStateException(__FILE__, __LINE__,
                                                     "<not>-rule has an invalid operand count.");
            }
            return !_rules[0]->eval(anno);
        }

        bool AfrNot::hasOperand() const {
            return hasChildren();
        }

        void AfrNot::setOperand(AnnoFilterRule *rule) {
            if(rule != NULL) {
                _rules.clear();
                addChild(rule);
            }
        }

        void AfrNot::setOperandClean(AnnoFilterRule *rule) {
            if (rule != NULL) {
                removeDelChild(0);
                addChild(rule);
            }
        }

        AnnoFilterRule *AfrNot::getOperand() {
            return getChild(0);
        }

        const AnnoFilterRule *AfrNot::getOperand() const {
            return getChild(0);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
