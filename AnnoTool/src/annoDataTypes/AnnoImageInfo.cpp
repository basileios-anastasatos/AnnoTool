#include "include/AnnoImageInfo.h"
#include "AnnoFileData.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        void AnnoImageInfo::setModified(bool mod) {
            bool prev = _modified;
            _modified = mod;
            if (_parFile != NULL) {
                _parFile->onAnnoImageInfoNotify(prev, mod);
            }
        }

        void AnnoImageInfo::setImagePath(const QString &path) {
            QFileInfo tmp(path);
            if (_imagePath != tmp) {
                _imagePath = tmp;
                setModified(true);
            }
        }

        void AnnoImageInfo::print() const {
            QTextStream out(stdout);
            out << "AnnoImageInfo..." << endl;
            out << "--------------------------------------" << endl;
            out << "Image-File: " << _imagePath.filePath() << endl;
            out << "Image-UUID: " << imageIdAsString() << endl;
            out << "Image-Source: " << _imageSource << endl;
            out << "Comment: " << _comment << endl;
            out << "--------------------------------------" << endl;
        }

        void AnnoImageInfo::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement("imageInfo");

            writer.writeStartElement("file");
            if (_frame != NOFRAME) {
                writer.writeAttribute("frame", QString::number(_frame, 10));
            }
            writer.writeCharacters(_imagePath.filePath());
            writer.writeEndElement();
            writer.writeTextElement("uuid", imageIdAsString());
            writer.writeTextElement("source", _imageSource);
            writer.writeTextElement("comment", _comment);
            writer.writeEndElement();
        }

        void AnnoImageInfo::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
            QString tagInfo("imageInfo");

            if (!reader.isStartElement() || reader.name() != tagInfo) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagInfo, reader.name().toString());
            }
            if (!XmlHelper::skipToStartElement("file", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "file", reader.name().toString());
            }
            QString strFrame = reader.attributes().value("frame").toString();
            if (!strFrame.isEmpty()) {
                bool ok = false;
                _frame = strFrame.toInt(&ok, 10);
                if (!ok) {
                    throw XmlHelper::genExpFormatAttr(__FILE__, __LINE__, "frame", strFrame);
                }
            }
            _imagePath = QFileInfo(reader.readElementText());

            if (!XmlHelper::skipToStartElement("uuid", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "uuid", reader.name().toString());
            }
            _imageId = QUuid(reader.readElementText());

            XmlHelper::skipToNextStartElement(true, reader);
            if (reader.isStartElement() && reader.name() == "source") {
                _imageSource = reader.readElementText();
                XmlHelper::skipToNextStartElement(true, reader);
            }
            if (reader.isStartElement() && reader.name() == "comment") {
                _comment = reader.readElementText();
            }

            XmlHelper::skipToEndElement(tagInfo, reader);
            reader.readNext();
        }

        AnnoImageInfo AnnoImageInfo::fromXml(QXmlStreamReader &reader) throw(XmlException *) {
            AnnoImageInfo data;
            data.loadFromXml(reader);
            return data;
        }

    } //end namespace dt
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
