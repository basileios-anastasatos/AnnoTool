#include "include/AnnoInfo.h"
#include "XmlHelper.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoInfo::AnnoInfo() {
        }

        AnnoInfo::~AnnoInfo() {
        }

        void AnnoInfo::setComplexId(const QUuid &uuid) {
            _annoComplex = uuid;
        }

        QUuid AnnoInfo::annoComplex() const {
            return _annoComplex;
        }

        void AnnoInfo::print() const {
            QTextStream out(stdout);
            out << "AnnoInfo..." << endl;
            out << "--------------------------------------" << endl;
            out << "AnnoComplex-UUID: " << XmlHelper::uuidAsString(_annoComplex) << endl;
            out << "--------------------------------------" << endl;
        }

        void AnnoInfo::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement("annotationInfo");
            writer.writeTextElement("annotationComplex",
                                    XmlHelper::uuidAsString(_annoComplex));
            writer.writeEndElement();
        }

        void AnnoInfo::loadFromXml(QXmlStreamReader &reader)
        throw(XmlFormatException *) {
            QString tagInfo("annotationInfo");
            QString tagCmplx("annotationComplex");

            if (!reader.isStartElement() || reader.name() != tagInfo) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML. Was expecting annotationInfo");
            }
            if (!XmlHelper::skipToStartElement(tagCmplx, reader)) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML. Was expecting annotationComplex");
            }

            QString val = reader.readElementText();
            _annoComplex = QUuid(val);

            XmlHelper::skipToEndElement(tagInfo, reader);
            reader.readNext();
        }

        AnnoInfo AnnoInfo::fromXml(QXmlStreamReader &reader)
        throw(XmlFormatException *) {
            AnnoInfo data;
            data.loadFromXml(reader);
            return data;
        }

    } //end namespace dt
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
