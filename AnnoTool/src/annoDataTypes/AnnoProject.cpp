#include "include/AnnoProject.h"
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

        AnnoProject::AnnoProject(const QString &path) {
            _sourceFile = path;
        }

        AnnoProject::AnnoProject(const QString &path, const QUuid &uuid) {
            _sourceFile = path;
            _uuid = uuid;
        }

        AnnoProject::~AnnoProject() {
        }

        void AnnoProject::addToClassPath(const QString &file) {
            QFileInfo info(file);
            if (!_classPaths.contains(info)) {
                _classPaths.append(info);
            }
        }

        void AnnoProject::addToSearchPath(const QString &dir) {
            QFileInfo info(dir);
            if (!_searchPaths.contains(info)) {
                _searchPaths.append(info);
            }
        }

        void AnnoProject::addToLinks(const QString &uuid) {
            QUuid id(uuid);
            if (!_links.contains(id)) {
                _links.append(id);
            }
        }

        bool AnnoProject::containsInClassPath(const QString &file) const {
            QFileInfo info(file);
            return _classPaths.contains(info);
        }

        bool AnnoProject::containsInSearchPath(const QString &dir) const {
            QFileInfo info(dir);
            return _searchPaths.contains(info);
        }

        bool AnnoProject::containsInLinks(const QString &uuid) const {
            QUuid id(uuid);
            return _links.contains(id);
        }

        QList<QUuid> *AnnoProject::links() {
            return &_links;
        }

        QList<QFileInfo> *AnnoProject::searchPath() {
            return &_searchPaths;
        }

        QList<QFileInfo> *AnnoProject::classPath() {
            return &_classPaths;
        }

        QString AnnoProject::filePath() const {
            return _sourceFile;
        }

        void AnnoProject::setFilePath(const QString &path) {
            _sourceFile = path;
        }

        QString AnnoProject::projectName() const {
            return _projectName;
        }

        void AnnoProject::setProjectName(const QString &name) {
            _projectName = name;
        }

        void AnnoProject::setUuid(const QUuid &uuid) {
            _uuid = uuid;
        }

        QUuid AnnoProject::uuid() const {
            return _uuid;
        }

        QString AnnoProject::uuidAsString() const {
            return XmlHelper::uuidAsString(_uuid);
        }

        void AnnoProject::loadFromFile() throw(IOException *, XmlException *) {
            QFile file(_sourceFile);
            if (!file.exists()) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot load from [%1]. File does not exist.").arg(_sourceFile));
            } else if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot load from [%1]. File cannot be opened.").arg(_sourceFile));
            }

            QXmlStreamReader reader(&file);
            reader.setNamespaceProcessing(true);
            if (!XmlHelper::skipToStartElement("annoProject", reader)) {
                throw new XmlFormatException(__FILE__, __LINE__, QString("Cannot load from [%1]. Invalid XML format.").arg(_sourceFile));
            }

            loadFromXml(reader);
            file.close();
        }

        void AnnoProject::loadFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != "annoProject") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "annoProject", reader.name().toString());
            }

            QString name = reader.attributes().value("name").toString();
            if (name.isEmpty()) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid ProjectName.");
            }
            _projectName = name;

            QString uuid = reader.attributes().value("uuid").toString();
            if (uuid.isEmpty()) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid UUID.");
            }
            _uuid = QUuid(uuid);

            XmlHelper::skipToNextStartElement(true, reader);
            if (reader.isStartElement() && reader.name() == "classPath") {
                loadClassPath(reader);
                XmlHelper::skipToNextStartElement(false, reader);
                if (reader.isStartElement() && reader.name() == "link") {
                    loadLinks(reader);
                    XmlHelper::skipToNextStartElement(false, reader);
                    if (reader.isStartElement() && reader.name() == "searchPath") {
                        loadSearchPath(reader);
                    } else {
                        throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting searchPath");
                    }
                } else if (reader.isStartElement() && reader.name() == "searchPath") {
                    loadSearchPath(reader);
                } else {
                    throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting link or searchPath");
                }
            } else if (reader.isStartElement() && reader.name() == "link") {
                loadLinks(reader);
                XmlHelper::skipToNextStartElement(false, reader);
                if (reader.isStartElement() && reader.name() == "searchPath") {
                    loadSearchPath(reader);
                } else {
                    throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting searchPath");
                }
            } else if (reader.isStartElement() && reader.name() == "searchPath") {
                loadSearchPath(reader);
            } else {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting classPath, link or searchPath");
            }
        }

        void AnnoProject::writeToFile() const throw(IOException *, XmlException *) {
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

        AnnoProject *AnnoProject::fromFile(const QString &path) throw(IOException *,
                XmlException *) {
            AnnoProject *data = new AnnoProject(path);
            data->loadFromFile();
            return data;
        }

        void AnnoProject::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("annoProject");
            writer.writeAttribute("name", _projectName);
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
                    writer.writeTextElement("uuid", XmlHelper::uuidAsString(i.next()));
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

        void AnnoProject::loadClassPath(QXmlStreamReader &reader)
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

        void AnnoProject::loadSearchPath(QXmlStreamReader &reader)
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

        void AnnoProject::loadLinks(QXmlStreamReader &reader) throw(XmlException *) {
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

        void AnnoProject::print() const {
            QTextStream out(stdout);
            out << "UUID: " << uuidAsString() << endl;
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
                    out << "UUID: " << XmlHelper::uuidAsString(iLink.next()) << endl;
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
