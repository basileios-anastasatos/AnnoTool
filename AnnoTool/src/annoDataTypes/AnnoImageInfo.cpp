#include "include/AnnoImageInfo.h"
#include "XmlHelper.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoImageInfo::AnnoImageInfo() {
        }

        AnnoImageInfo::~AnnoImageInfo() {
        }

        QFileInfo AnnoImageInfo::imagePath() const {
            return _imagePath;
        }

        QUuid AnnoImageInfo::imageId() const {
            return _imageId;
        }

        QString AnnoImageInfo::imageIdAsString() const {
            return XmlHelper::uuidAsString(_imageId);
        }

        QString AnnoImageInfo::imageSource() const {
            return _imageSource;
        }

        QString AnnoImageInfo::comment() const {
            return _comment;
        }

        void AnnoImageInfo::setImagePath(const QString &path) {
            _imagePath = QFileInfo(path);
        }

        void AnnoImageInfo::setImagePath(const QFileInfo &path) {
            _imagePath = path;
        }

        void AnnoImageInfo::setImageId(const QUuid &id) {
            _imageId = id;
        }

        void AnnoImageInfo::setImageSource(const QString &source) {
            _imageSource = source;
        }

        void AnnoImageInfo::setComment(const QString &comment) {
            _comment = comment;
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

        void AnnoImageInfo::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("imageInfo");
            writer.writeTextElement("file", _imagePath.filePath());
            writer.writeTextElement("uuid", imageIdAsString());
            writer.writeTextElement("source", _imageSource);
            writer.writeTextElement("comment", _comment);
            writer.writeEndElement();
        }

        void AnnoImageInfo::loadFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            QString tagInfo("imageInfo");

            if (!reader.isStartElement() || reader.name() != tagInfo) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagInfo, reader.name().toString());
            }
            if (!XmlHelper::skipToStartElement("file", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "file", reader.name().toString());
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

        AnnoImageInfo AnnoImageInfo::fromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            AnnoImageInfo data;
            data.loadFromXml(reader);
            return data;
        }

    } //end namespace dt
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
