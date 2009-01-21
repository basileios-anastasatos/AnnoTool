#include "include/AfrNot.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;

namespace anno {
    namespace filter {
        const QString AfrNot::XML_NAME("not");

        AfrNot::AfrNot(bool autoDelete) :
            LogicFilterRule(autoDelete) {
            // nothing to do here
        }

        AfrNot::AfrNot(AnnoFilterRule *rule, bool autoDelete) :
            LogicFilterRule(autoDelete) {
            _rules.append(rule);
        }

        AfrNot::~AfrNot() {
        }

        AfrNot *AfrNot::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            AfrNot *pRule = new AfrNot(true);

            try {
                pRule->loadFromXml(reader);
            } catch(exc::XmlException *e) {
                delete pRule;
                throw e;
            }

            return pRule;
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
            writer.writeStartElement(XML_NAME);
            LogicFilterRule::toXmlInternal(writer);
            writer.writeEndElement();
        }

        void AfrNot::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || !isXmlName(curParent)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_NAME, curParent);
            }

            XmlHelper::skipToNextStartElement(true, reader);
            AnnoFilterRule *pRule = AnnoFilterXmlLoader::loadRule(reader);
            if(pRule == NULL) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, QString("Encountered unknown Filter Rule Tag <%1>").arg(reader.name().toString()));
            }
            addChild(pRule);

            //XmlHelper::skipToEndElement(XML_NAME, reader);
            XmlHelper::skipToNextEndElement(false, reader);
            if(!reader.isEndElement() || !isXmlName(reader.name().toString())) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, QString("Encountered invalid tag <%1>, but expected </%2>").arg(reader.name().toString()).arg(XML_NAME));
            }
            reader.readNext();
        }

        bool AfrNot::evalInternal(const dt::Annotation *anno) const
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
