#include "include/AfrAttributeValue.h"

namespace anno {
    namespace filter {

        AfrAttributeValue::AfrAttributeValue(const QString &attrName, const QString &attrValue) :
            AnnoFilterRuleAtom(true), _attrName(attrName), _attrValue(attrValue) {
        }

        AfrAttributeValue::AfrAttributeValue(const QString &attrClass, const QString &attrName, const QString &attrValue) :
            AnnoFilterRuleAtom(true), _attrClass(attrClass), _attrName(attrName), _attrValue(attrValue) {
        }

        AfrAttributeValue::~AfrAttributeValue() {
            printf("delete <AfrAttributeValue>\n");
        }

        bool AfrAttributeValue::isValid() const {
            return !_attrName.isEmpty();
        }

        QString AfrAttributeValue::toString() const {
            QString str("<Has AttributeValue [%1 = %2]>");
            str = str.arg(_attrName).arg(_attrValue);
            return str;
        }

        void AfrAttributeValue::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeEmptyElement("hasAttribute");
            writer.writeAttribute("name", _attrName);
            if(!_attrClass.isEmpty()) {
                writer.writeAttribute("class", _attrClass);
            }
            writer.writeAttribute("value", _attrValue);
        }

        void AfrAttributeValue::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            //TODO
        }

        bool AfrAttributeValue::eval(const dt::Annotation *anno) const
        throw(exc::IllegalStateException *) {
            if(_attrName.isEmpty()) {
                throw new exc::IllegalStateException(__FILE__, __LINE__, "<HasAttribute>-rule has invalid config.");
            }

            QList<dt::AnnoAttribute> attrList = anno->attributes();
            if(!_attrClass.isEmpty()) {
                foreach(dt::AnnoAttribute attr, attrList) {
                    if(attr.name() == _attrName && attr.className() == _attrClass && attr.value() == _attrValue) {
                        return true;
                    }
                }
            } else {
                foreach(dt::AnnoAttribute attr, attrList) {
                    if(attr.name() == _attrName && (_attrValue.isNull() ? true : attr.value() == _attrValue)) {
                        return true;
                    }
                }
            }
            return false;
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
