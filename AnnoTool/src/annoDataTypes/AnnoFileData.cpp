#include "include/AnnoFileData.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QTextStream>
#include "XmlHelper.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoFileData::AnnoFileData(const QString &path) {
            _sourceFile = path;
        }

        AnnoFileData::~AnnoFileData() {
            if (!_annoList.isEmpty()) {
                QListIterator<Annotation *> i(_annoList);
                while (i.hasNext()) {
                    delete i.next();
                }
            }
        }

        AnnoImageInfo *AnnoFileData::imageInfo() {
            return &_imageInfo;
        }

        AnnoInfo *AnnoFileData::annoInfo() {
            return &_annoInfo;
        }

        QList<Annotation *> *AnnoFileData::annoList() {
            return &_annoList;
        }

        QString AnnoFileData::filePath() const {
            return _sourceFile;
        }

        void AnnoFileData::setFilePath(const QString &path) {
            _sourceFile = path;
        }

        bool AnnoFileData::containsAnnotation(const QUuid &uuid) const {
            QListIterator<Annotation *> i(_annoList);
            while (i.hasNext()) {
                if (i.next()->annoId() == uuid) {
                    return true;
                }
            }
            return false;
        }

        Annotation *AnnoFileData::getAnnotation(const QUuid &uuid) {
            QListIterator<Annotation *> i(_annoList);
            while (i.hasNext()) {
                Annotation *a = i.next();
                if (a->annoId() == uuid) {
                    return a;
                }
            }
            return NULL;
        }

        QUuid AnnoFileData::imageUuid() const {
            return _imageInfo.imageId();
        }

        QString AnnoFileData::imageUuidAsString() const {
            return XmlHelper::uuidAsString(_imageInfo.imageId());
        }

        QUuid AnnoFileData::complexUuid() const {
            return _annoInfo.annoComplex();
        }

        QString AnnoFileData::complexUuidAsString() const {
            return XmlHelper::uuidAsString(_annoInfo.annoComplex());
        }

        void AnnoFileData::print() const {
            QTextStream out(stdout);
            out << "AnnoFileData from" << endl << _sourceFile << endl;
            out << "--------------------------------------------------------" << endl;
            _imageInfo.print();
            _annoInfo.print();

            out << "Annotations..." << endl;
            out << "---------------------------" << endl;
            QListIterator<Annotation *> i(_annoList);
            while (i.hasNext()) {
                i.next()->print();
            }
            out << "--------------------------------------------------------" << endl;
        }

        void AnnoFileData::loadFromFile() throw(IOException *, XmlException *) {
            QFile file(_sourceFile);
            if (!file.exists()) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot load from [%1]. File does not exist.").arg(_sourceFile));
            } else if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot load from [%1]. File cannot be opened.").arg(_sourceFile));
            }

            QXmlStreamReader reader(&file);
            reader.setNamespaceProcessing(true);
            if (!XmlHelper::skipToStartElement("imageAnnotationFile", reader)) {
                throw new XmlFormatException(__FILE__, __LINE__, QString("Cannot load from [%1]. Invalid XML format.").arg(_sourceFile));
            }

            loadFromXml(reader);
            file.close();
        }

        void AnnoFileData::loadFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != "imageAnnotationFile") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "imageAnnotationFile", reader.name().toString());
            }

            if (!XmlHelper::skipToStartElement("imageInfo", reader)) {
                throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "imageInfo", reader.name().toString());
            }
            _imageInfo = AnnoImageInfo::fromXml(reader);

            if (!XmlHelper::skipToStartElement("annotationInfo", reader)) {
                throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "annotationInfo", reader.name().toString());
            }
            _annoInfo = AnnoInfo::fromXml(reader);

            if (!XmlHelper::skipToStartElement("imageAnnotations", reader)) {
                throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "imageAnnotations", reader.name().toString());
            }

            QString tagAnnoLst("imageAnnotations");
            QString tagAnno("annotation");
            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagAnno) {
                    _annoList.append(Annotation::fromXml(reader));
                } else if (reader.isEndElement() && reader.name() == tagAnnoLst) {
                    reader.readNext();
                    break;
                } else {
                    reader.readNext();
                }
            }
        }

        void AnnoFileData::writeToFile() const throw(IOException *, XmlException *) {
            QFile file(_sourceFile);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot write to [%1]. File cannot be opened.").arg(_sourceFile));
            }

            QXmlStreamWriter writer(&file);
            writer.setAutoFormatting(true);
            writer.writeStartDocument();
            toXml(writer);
            writer.writeEndDocument();
            file.flush();
            file.close();
        }

        AnnoFileData *AnnoFileData::fromFile(const QString &path)
        throw(IOException *, XmlException *) {
            AnnoFileData *data = new AnnoFileData(path);
            data->loadFromFile();
            return data;
        }

        void AnnoFileData::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("imageAnnotationFile");
            _imageInfo.toXml(writer);
            _annoInfo.toXml(writer);
            writer.writeStartElement("imageAnnotations");
            if (!_annoList.isEmpty()) {
                QListIterator<Annotation *> i(_annoList);
                while (i.hasNext()) {
                    i.next()->toXml(writer);
                }
            }
            writer.writeEndElement();
            writer.writeEndElement();
        }

        AnnoFileData *AnnoFileData::fromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            AnnoFileData *data = new AnnoFileData("unknown");
            data->loadFromXml(reader);
            data->_sourceFile = data->imageInfo()->imageIdAsString();
            return data;
        }

    } //end namespace dt
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
