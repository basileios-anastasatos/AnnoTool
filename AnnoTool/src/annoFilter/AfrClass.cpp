#include "include/AfrClass.h"
#include "Annotation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace anno {
    namespace filter {

        AfrClass::AfrClass(const QString &className) :
            AnnoFilterRuleAtom(true), _className(className) {
        }

        AfrClass::~AfrClass() {
            printf("delete <AfrClass>\n");
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
            writer.writeEmptyElement("hasClass");
            writer.writeAttribute("name", _className);
        }

        void AfrClass::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            //TODO
        }

        bool AfrClass::eval(const dt::Annotation *anno) const throw(exc::IllegalStateException *) {
            if(_className.isEmpty()) {
                throw new exc::IllegalStateException(__FILE__, __LINE__, "<hasClass>-rule has invalid config.");
            }
            return (anno->classes().indexOf(_className) >= 0);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
