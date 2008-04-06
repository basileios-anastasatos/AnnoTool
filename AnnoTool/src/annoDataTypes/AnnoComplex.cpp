#include "AnnoComplex.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QListIterator>
#include "XmlHelper.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoComplex::AnnoComplex() {
        }

        AnnoComplex::AnnoComplex(const QUuid &uuid) {
            _uuid = uuid;
        }

        AnnoComplex::~AnnoComplex() {
        }

        void AnnoComplex::addToClassPath(const QString &file) {
            QFileInfo info(file);
            if (!_classPaths.contains(info)) {
                _classPaths.append(info);
            }
        }

        void AnnoComplex::addToSearchPath(const QString &dir) {
            QFileInfo info(dir);
            if (!_searchPaths.contains(info)) {
                _searchPaths.append(info);
            }
        }

        void AnnoComplex::addToLinks(const QString &uuid) {
            QUuid id(uuid);
            if (!_links.contains(id)) {
                _links.append(id);
            }
        }

        bool AnnoComplex::containsInClassPath(const QString &file) const {
            QFileInfo info(file);
            return _classPaths.contains(info);
        }

        bool AnnoComplex::containsInSearchPath(const QString &dir) const {
            QFileInfo info(dir);
            return _searchPaths.contains(info);
        }

        bool AnnoComplex::containsInLinks(const QString &uuid) const {
            QUuid id(uuid);
            return _links.contains(id);
        }

        const QList<QUuid> *AnnoComplex::links() {
            return &_links;
        }

        const QList<QFileInfo> *AnnoComplex::searchPath() {
            return &_searchPaths;
        }

        const QList<QFileInfo> *AnnoComplex::classPath() {
            return &_classPaths;
        }

        void AnnoComplex::setUuid(const QUuid &uuid) {
            _uuid = uuid;
        }

        QUuid AnnoComplex::uuid() const {
            return _uuid;
        }

        QString AnnoComplex::uuidAsString() const {
            return uuidAsString(_uuid);
        }

        QString AnnoComplex::uuidAsString(const QUuid &uuid) {
            QString str = uuid.toString();
            str.remove(0, 1);
            str.remove(str.length() - 1, 1);
            return str;
        }

        void AnnoComplex::writeToFile(const QString &path) const
        throw(IOException *, XmlFormatException *) {
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot write to [%1]. File cannot be opened.").arg(path));
            }

            QXmlStreamWriter writer(&file);
            writer.setAutoFormatting(true);
            writer.writeStartDocument();
            toXml(writer);
            writer.writeEndDocument();
            file.flush();
            file.close();
        }

        void AnnoComplex::toXml(QXmlStreamWriter &writer) const
        throw(XmlFormatException *) {
            writer.writeStartElement("annoComplex");
            writer.writeAttribute("uuid", uuidAsString());
            if (!_classPaths.isEmpty()) {
                writer.writeStartElement("classPath");
                QListIterator<QFileInfo> i(_classPaths);
                while (i.hasNext()) {
                    writer.writeTextElement("file", i.next().filePath());
                }
                writer.writeEndElement();
            }
            if (!_links.isEmpty()) {
                writer.writeStartElement("link");
                QListIterator<QUuid> i(_links);
                while (i.hasNext()) {
                    writer.writeTextElement("uuid", uuidAsString(i.next()));
                }
                writer.writeEndElement();
            }
            if (_searchPaths.isEmpty()) {
                throw new XmlFormatException(__FILE__, __LINE__, "No search path elements. Writing this XML would result in invalid XML file structure.");
            } else {
                writer.writeStartElement("searchPath");
                QListIterator<QFileInfo> i(_searchPaths);
                while (i.hasNext()) {
                    writer.writeTextElement("dir", i.next().filePath());
                }
                writer.writeEndElement();
            }
            writer.writeEndElement();
        }

        AnnoComplex AnnoComplex::loadFromFile(const QString &path)
        throw(IOException *, XmlException *) {
            QFile file(path);
            if (!file.exists()) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot load from [%1]. File does not exist.").arg(path));
            } else if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot load from [%1]. File cannot be opened.").arg(path));
            }

            QXmlStreamReader reader(&file);
            reader.setNamespaceProcessing(true);
            if (!XmlHelper::skipToStartElement("annoComplex", reader)) {
                throw new XmlFormatException(__FILE__, __LINE__, QString("Cannot load from [%1]. Invalid XML format.").arg(path));
            }

            QString uuid = reader.attributes().value("uuid").toString();
            if (uuid.isEmpty()) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid UUID.");
            }
            AnnoComplex anno;
            anno._uuid = QUuid(uuid);

            XmlHelper::skipToNextStartElement(true, reader);
            if (reader.isStartElement() && reader.name() == "classPath") {
                anno.loadClassPath(reader);
                XmlHelper::skipToNextStartElement(false, reader);
                if (reader.isStartElement() && reader.name() == "link") {
                    anno.loadLinks(reader);
                    XmlHelper::skipToNextStartElement(false, reader);
                    if (reader.isStartElement() && reader.name() == "searchPath") {
                        anno.loadSearchPath(reader);
                    } else {
                        throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting searchPath");
                    }
                } else if (reader.isStartElement() && reader.name() == "searchPath") {
                    anno.loadSearchPath(reader);
                } else {
                    throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting link or searchPath");
                }
            } else if (reader.isStartElement() && reader.name() == "link") {
                anno.loadLinks(reader);
                XmlHelper::skipToNextStartElement(false, reader);
                if (reader.isStartElement() && reader.name() == "searchPath") {
                    anno.loadSearchPath(reader);
                } else {
                    throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting searchPath");
                }
            } else if (reader.isStartElement() && reader.name() == "searchPath") {
                anno.loadSearchPath(reader);
            } else {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting classPath, link or searchPath");
            }

            return anno;
        }

        void AnnoComplex::loadClassPath(QXmlStreamReader &reader)
        throw(XmlException *) {
            QString tagList("classPath");
            QString tagFile("file");

            if (!reader.isStartElement() || reader.name() != tagList) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML. Was expecting classPath");
            }

            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagFile) {
                    QString val = reader.readElementText();
                    addToClassPath(val);
                } else if (reader.isEndElement() && reader.name() == tagList) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }

        void AnnoComplex::loadSearchPath(QXmlStreamReader &reader)
        throw(XmlException *) {
            QString tagList("searchPath");
            QString tagDir("dir");

            if (!reader.isStartElement() || reader.name() != tagList) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML. Was expecting searchPath");
            }

            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagDir) {
                    QString val = reader.readElementText();
                    addToSearchPath(val);
                } else if (reader.isEndElement() && reader.name() == tagList) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }

        void AnnoComplex::loadLinks(QXmlStreamReader &reader) throw(XmlException *) {
            QString tagList("link");
            QString tagUuid("uuid");

            if (!reader.isStartElement() || reader.name() != tagList) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML. Was expecting link");
            }

            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagUuid) {
                    QString val = reader.readElementText();
                    addToLinks(val);
                } else if (reader.isEndElement() && reader.name() == tagList) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }

        void AnnoComplex::print() const {
            QTextStream out(stdout);
            out << "UUID: " << uuidAsString(_uuid) << endl;
            out << "-------------------------" << endl;

            if (!_classPaths.isEmpty()) {
                out << "<< Class-Path >>" << endl;
                QListIterator<QFileInfo> iClass(_classPaths);
                while (iClass.hasNext()) {
                    out << "File: " << iClass.next().filePath() << endl;
                }
            }

            if (!_links.isEmpty()) {
                out << endl << "<< Links >>" << endl;
                QListIterator<QUuid> iLink(_links);
                while (iLink.hasNext()) {
                    out << "UUID: " << uuidAsString(iLink.next()) << endl;
                }
                out << endl;
            }

            if (!_searchPaths.isEmpty()) {
                out << endl << "<< Search-Path >>" << endl;
                QListIterator<QFileInfo> iSearch(_searchPaths);
                while (iSearch.hasNext()) {
                    out << "File: " << iSearch.next().filePath() << endl;
                }
            }
            out << "-------------------------" << endl;
        }

    } //end namespace dt
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
