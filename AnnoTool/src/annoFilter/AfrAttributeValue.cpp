#include "include/AfrAttributeValue.h"
#include "Annotation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "importGlobals.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;


namespace anno {
    namespace filter {
        const QString AfrAttributeValue::XML_NAME("hasAttr");

        AfrAttributeValue::AfrAttributeValue() :
            AnnoFilterRuleAtom(true) {
        }

        AfrAttributeValue::AfrAttributeValue(const QString &attrName, bool caseSensitive, bool autoDelete) :
            AnnoFilterRuleAtom(autoDelete), _attrName(attrName), _caseSensitive(caseSensitive) {
        }

        AfrAttributeValue::AfrAttributeValue(const QString &attrName, const QString &attrValue, bool caseSensitive, bool autoDelete) :
            AnnoFilterRuleAtom(autoDelete), _attrName(attrName), _attrValue(attrValue), _caseSensitive(caseSensitive) {
        }

        AfrAttributeValue::AfrAttributeValue(const QString &attrClass, const QString &attrName, const QString &attrValue, bool caseSensitive, bool autoDelete) :
            AnnoFilterRuleAtom(autoDelete), _attrClass(attrClass), _attrName(attrName), _attrValue(attrValue), _caseSensitive(caseSensitive) {
        }

        AfrAttributeValue::~AfrAttributeValue() {
        }

        AfrAttributeValue *AfrAttributeValue::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            AfrAttributeValue *pRule = new AfrAttributeValue();

            try {
                pRule->loadFromXml(reader);
            } catch(exc::XmlException *e) {
                delete pRule;
                throw e;
            }

            return pRule;
        }

        bool AfrAttributeValue::isValid() const {
            return (!_attrName.isEmpty() && (_attrClass.isEmpty() || (!_attrClass.isEmpty() && !_attrValue.isEmpty())));
        }

        QString AfrAttributeValue::toString() const {
            QString str("<Has AttributeValue [%1 = %2][class=%3]>");
            str = str.arg(_attrName).arg(_attrValue).arg(_attrClass);
            return str;
        }

        void AfrAttributeValue::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeEmptyElement(XML_NAME);
            writer.writeAttribute("name", _attrName);
            if(!_attrClass.isEmpty()) {
                writer.writeAttribute("class", _attrClass);
            }
            if(!_attrValue.isEmpty()) {
                writer.writeAttribute("value", _attrValue);
            }
            if(!_caseSensitive) {
                writer.writeAttribute("cs", "0");
            }
        }

        void AfrAttributeValue::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || !isXmlName(curParent)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_NAME, curParent);
            }

            QString name = reader.attributes().value("name").toString();
            QString strClass = reader.attributes().value("class").toString();
            QString value = reader.attributes().value("value").toString();
            QString cs = reader.attributes().value("cs").toString();

            if(name.isEmpty()) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, "Attribute name must not be empty!");
            }
            _attrName = name;

            if(!strClass.isEmpty()) {
                if(value.isEmpty()) {
                    throw new exc::XmlFormatException(__FILE__, __LINE__, "Attribute value must not be empty because a class attribute was specified!");
                }
                _attrClass = strClass;
                _attrValue = value;
            } else {
                _attrClass = QString();
                _attrValue = value;
            }

            if(cs.isEmpty()) {
                _caseSensitive = true;
            } else {
                bool ok = false;
                int csn = cs.toInt(&ok);
                if (!ok) {
                    throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid cs flag data.");
                }
                _caseSensitive = (bool)csn;
            }
            XmlHelper::skipToEndElement(XML_NAME, reader);
            reader.readNext();
        }

        bool AfrAttributeValue::evalInternal(const dt::Annotation *anno) const
        throw(exc::IllegalStateException *) {
            if(_attrName.isEmpty()) {
                throw new exc::IllegalStateException(__FILE__, __LINE__, "<HasAttribute>-rule has invalid config.");
            }

            QList<dt::AnnoAttribute> attrList = anno->attributes();
            if(!_attrClass.isEmpty()) {
                foreach(dt::AnnoAttribute attr, attrList) {
                    if(QString::compare(attr.name(), _attrName, convCase()) == 0 &&
                            QString::compare(attr.className(), _attrClass, convCase()) == 0 &&
                            QString::compare(attr.value(), _attrValue, convCase()) == 0) {
                        return true;
                    }
                }
            } else {
                foreach(dt::AnnoAttribute attr, attrList) {
                    if(QString::compare(attr.name(), _attrName, convCase()) == 0 &&
                            _attrValue.isNull() ? true : QString::compare(attr.value(), _attrValue, convCase()) == 0) {
                        return true;
                    }
                }
            }
            return false;
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
