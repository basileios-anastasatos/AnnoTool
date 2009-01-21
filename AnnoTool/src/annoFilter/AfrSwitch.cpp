#include "include/AfrSwitch.h"
#include "Annotation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;

namespace anno {
    namespace filter {

        const QString AfrSwitch::XML_NAME("switch");

        AfrSwitch::AfrSwitch() :
            AnnoFilterRuleAtom(true, true, true) {
        }

        AfrSwitch::AfrSwitch(bool state, bool autoDelete) :
            AnnoFilterRuleAtom(true, state, autoDelete) {
        }

        AfrSwitch::~AfrSwitch() {
        }

        AfrSwitch *AfrSwitch::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            AfrSwitch *pRule = new AfrSwitch();

            try {
                pRule->loadFromXml(reader);
            } catch(exc::XmlException *e) {
                delete pRule;
                throw e;
            }

            return pRule;
        }

        bool AfrSwitch::isValid() const {
            return true;
        }

        QString AfrSwitch::toString() const {
            QString str("<Switch [%1]>");
            str = str.arg(_constValue ? "true" : "false");
            return str;
        }

        void AfrSwitch::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeEmptyElement(XML_NAME);
            writer.writeAttribute("state", _constValue ? "1" : "0");
        }

        void AfrSwitch::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || !isXmlName(curParent)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_NAME, curParent);
            }

            QString state = reader.attributes().value("state").toString();

            if(state.isEmpty()) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, "Switch state must not be empty!");
            }
            bool ok = false;
            int sw = state.toInt(&ok);
            if (!ok) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid switch state data.");
            }
            _constValue = (bool)sw;
            XmlHelper::skipToEndElement(XML_NAME, reader);
            reader.readNext();
        }

        bool AfrSwitch::evalInternal(const dt::Annotation *anno) const throw(exc::IllegalStateException *) {
            return _constValue;
        }


    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
