#include "include/AfrAnd.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;

namespace anno {

    namespace filter {
        const QString AfrAnd::XML_NAME("and");

        AfrAnd::AfrAnd(bool autoDelete) : LogicFilterRule(autoDelete) {
            // nothing to do here
        }

        AfrAnd::~AfrAnd() {
            printf("delete <and>\n");
        }

        AfrAnd *AfrAnd::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            AfrAnd *pRule = new AfrAnd(true);

            try {
                pRule->loadFromXml(reader);
            } catch(exc::XmlException *e) {
                delete pRule;
                throw e;
            }

            return pRule;
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
            writer.writeStartElement(XML_NAME);
            LogicFilterRule::toXml(writer);
            writer.writeEndElement();
        }

        void AfrAnd::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
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
