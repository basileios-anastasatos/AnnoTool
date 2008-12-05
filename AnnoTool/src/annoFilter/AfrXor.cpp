#include "include/AfrXor.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;

namespace anno {
    namespace filter {
        const QString AfrXor::XML_NAME("xor");

        AfrXor::AfrXor(bool autoDelete) :
            LogicFilterRule(autoDelete) {
            // nothing to do here
        }

        AfrXor::~AfrXor() {
            printf("delete <xor>\n");
        }

        AfrXor *AfrXor::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            AfrXor *pRule = new AfrXor(true);

            try {
                pRule->loadFromXml(reader);
            } catch(exc::XmlException *e) {
                delete pRule;
                throw e;
            }

            return pRule;
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
            writer.writeStartElement(XML_NAME);
            LogicFilterRule::toXml(writer);
            writer.writeEndElement();
        }

        void AfrXor::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || !isXmlName(curParent)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_NAME, curParent);
            }

            XmlHelper::skipToNextStartElement(true, reader);
            while(!reader.atEnd()) {
                if(reader.isStartElement()) {
                    AnnoFilterRule *pRule = AnnoFilterXmlLoader::loadRule(reader);
                    if(pRule == NULL) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, QString("Encountered unknown Filter Rule Tag <%1>").arg(reader.name().toString()));
                    }
                    addChild(pRule);
                } else if(reader.isEndElement() && isXmlName(reader.name().toString())) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
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
