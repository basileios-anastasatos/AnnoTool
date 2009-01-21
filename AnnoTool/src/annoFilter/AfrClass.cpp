#include "include/AfrClass.h"
#include "Annotation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;

namespace anno {
    namespace filter {
        const QString AfrClass::XML_NAME("hasClass");

        AfrClass::AfrClass() :
            AnnoFilterRuleAtom(true) {
        }

        AfrClass::AfrClass(const QString &className, bool caseSensitive, bool autoDelete) :
            AnnoFilterRuleAtom(autoDelete), _className(className), _caseSensitive(caseSensitive) {
        }

        AfrClass::~AfrClass() {
        }

        AfrClass *AfrClass::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            AfrClass *pRule = new AfrClass();

            try {
                pRule->loadFromXml(reader);
            } catch(exc::XmlException *e) {
                delete pRule;
                throw e;
            }

            return pRule;
        }

        bool AfrClass::isValid() const {
            return !_className.isEmpty();
        }

        QString AfrClass::toString() const {
            QString str("<Has Class [%1]>");
            str = str.arg(_className);
            return str;
        }

        void AfrClass::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeEmptyElement(XML_NAME);
            writer.writeAttribute("name", _className);
        }

        void AfrClass::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || !isXmlName(curParent)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_NAME, curParent);
            }

            QString name = reader.attributes().value("name").toString();

            if(name.isEmpty()) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, "Class name must not be empty!");
            }
            _className = name;
            XmlHelper::skipToEndElement(XML_NAME, reader);
            reader.readNext();
        }

        bool AfrClass::evalInternal(const dt::Annotation *anno) const throw(exc::IllegalStateException *) {
            if(_className.isEmpty()) {
                throw new exc::IllegalStateException(__FILE__, __LINE__, "<hasClass>-rule has invalid config.");
            }
            return (anno->classes().indexOf(_className) >= 0);
            //TODO think about the use of case sensitivity settings here!
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
