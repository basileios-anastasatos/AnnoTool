#include "include/AnnoProject.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QListIterator>
#include "XmlHelper.h"
#include "GlobalLogger.h"
#include "GlobalProjectManager.h"
#include "AnnoFilterManager.h"

//namespace AnnoTool
namespace anno {

    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        const QString AnnoProject::XML_PROJECT("annoProject");
        const QString AnnoProject::XML_CLASSPATH("classPath");
        const QString AnnoProject::XML_SEARCHPATH("searchPath");
        const QString AnnoProject::XML_FILTERS("annoFilters");
        const QString AnnoProject::XML_COLORRULES("annoColorRules");
        const QString AnnoProject::XML_LINK("link");

        AnnoProject::AnnoProject(const QString &path) {
            _sourceFile = path;
        }

        AnnoProject::AnnoProject(const QString &path, const QUuid &uuid) {
            _sourceFile = path;
            _uuid = uuid;
        }

        AnnoProject::~AnnoProject() {
            if(!colorRules()->isEmpty()) {
                qDeleteAll(_colorRules);
            }
            if(!filters()->isEmpty()) {
                qDeleteAll(_filters);
            }
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

        QMap<QString, filter::AnnoFilter *> *AnnoProject::filters() {
            return &_filters;
        }

        QList<filter::ColorFilterEntry *> *AnnoProject::colorRules() {
            return &_colorRules;
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
            if (!XmlHelper::skipToStartElement(XML_PROJECT, reader)) {
                throw new XmlFormatException(__FILE__, __LINE__, QString("Cannot load from [%1]. Invalid XML format.").arg(_sourceFile));
            }

            loadFromXml(reader);
            file.close();
        }

        bool AnnoProject::isXmlComponent(const QString &cmpName) {
            return (cmpName == XML_SEARCHPATH ||
                    cmpName == XML_CLASSPATH  ||
                    cmpName == XML_FILTERS    ||
                    cmpName == XML_LINK       ||
                    cmpName == XML_COLORRULES
                   );
        }

        bool AnnoProject::isXmlComponent(const QStringRef &cmpName) {
            return (cmpName == XML_SEARCHPATH ||
                    cmpName == XML_CLASSPATH  ||
                    cmpName == XML_FILTERS    ||
                    cmpName == XML_LINK       ||
                    cmpName == XML_COLORRULES
                   );
        }

        void AnnoProject::loadFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != XML_PROJECT) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_PROJECT, reader.name().toString());
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

            bool sawSearchPath = false;
            bool sawClassPath = false;
            bool sawLink = false;

            XmlHelper::skipToNextStartElement(true, reader);
            while(isXmlComponent(reader.name())) {
                QStringRef curName = reader.name();
                if(curName == XML_SEARCHPATH) {
                    if(sawSearchPath) {
                        throw new XmlFormatException(__FILE__, __LINE__, QString("Invalid XML structure, double <%1> tag").arg(XML_SEARCHPATH));
                    }

                    loadSearchPath(reader);
                    sawSearchPath = true;
                } else if(curName == XML_CLASSPATH) {
                    if(sawClassPath) {
                        throw new XmlFormatException(__FILE__, __LINE__, QString("Invalid XML structure, double <%1> tag").arg(XML_CLASSPATH));
                    }

                    loadClassPath(reader);
                    sawClassPath = true;
                } else if(curName == XML_FILTERS) {
                    loadFilters(reader);
                } else if(curName == XML_LINK) {
                    if(sawLink) {
                        throw new XmlFormatException(__FILE__, __LINE__, QString("Invalid XML structure, double <%1> tag").arg(XML_LINK));
                    }

                    loadLinks(reader);
                    sawLink = true;
                } else if (curName == XML_COLORRULES) {
                    loadColorRules(reader);
                }

                XmlHelper::skipToNextStartElement(false, reader);
            }

            if(!sawSearchPath) {
                throw new XmlFormatException(__FILE__, __LINE__, QString("Invalid XML structure, was expecting a <%1> tag but found none.").arg(XML_SEARCHPATH));
            }
            if(reader.isStartElement()) {
                throw new XmlFormatException(__FILE__, __LINE__, QString("Invalid XML structure, was expecting no more starting tags but found <%1>.").arg(reader.name().toString()));
            }


//			if (reader.isStartElement() && reader.name() == "classPath")
//			{
//				loadClassPath(reader);
//				XmlHelper::skipToNextStartElement(false, reader);
//				if (reader.isStartElement() && reader.name() == "link")
//				{
//					loadLinks(reader);
//					XmlHelper::skipToNextStartElement(false, reader);
//					if (reader.isStartElement() && reader.name() == "searchPath")
//					{
//						loadSearchPath(reader);
//					}
//					else
//					{
//						throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting searchPath");
//					}
//				}
//				else if (reader.isStartElement() && reader.name() == "searchPath")
//				{
//					loadSearchPath(reader);
//				}
//				else
//				{
//					throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting link or searchPath");
//				}
//			}
//			else if (reader.isStartElement() && reader.name() == "link")
//			{
//				loadLinks(reader);
//				XmlHelper::skipToNextStartElement(false, reader);
//				if (reader.isStartElement() && reader.name() == "searchPath")
//				{
//					loadSearchPath(reader);
//				}
//				else
//				{
//					throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting searchPath");
//				}
//			}
//			else if (reader.isStartElement() && reader.name() == "searchPath")
//			{
//				loadSearchPath(reader);
//			}
//			else if (reader.isStartElement() && reader.name() == "annoFilters")
//			{
//				loadFilters(reader);
//			}
//			else
//			{
//				throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, was expecting classPath, link or searchPath");
//			}
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

        AnnoProject *AnnoProject::fromFile(const QString &path, filter::AnnoFilterManager *filterMan)
                throw(IOException *, XmlException *) {
            AnnoProject *data = new AnnoProject(path);
            filterMan->setProject(data); // Complete the initialization of the filter manager
            data->loadFromFile();
            return data;
        }

        void AnnoProject::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement(XML_PROJECT);
            writer.writeAttribute("name", _projectName);
            writer.writeAttribute("uuid", uuidAsString());
            if (!_classPaths.isEmpty()) {
                writer.writeStartElement(XML_CLASSPATH);
                QListIterator<QFileInfo> i(_classPaths);
                while (i.hasNext()) {
                    writer.writeTextElement("file", i.next().filePath());
                }
                writer.writeEndElement();
            }
            if (!_links.isEmpty()) {
                writer.writeStartElement(XML_LINK);
                QListIterator<QUuid> i(_links);
                while (i.hasNext()) {
                    writer.writeTextElement("uuid", XmlHelper::uuidAsString(i.next()));
                }
                writer.writeEndElement();
            }
            if (_searchPaths.isEmpty()) {
                throw new XmlFormatException(__FILE__, __LINE__, "No search path elements. Writing this XML would result in invalid XML file structure.");
            } else {
                writer.writeStartElement(XML_SEARCHPATH);
                QListIterator<QFileInfo> i(_searchPaths);
                while (i.hasNext()) {
                    writer.writeTextElement("dir", i.next().filePath());
                }
                writer.writeEndElement();
            }
            if(!_filters.isEmpty()) {
                saveFilters(writer);
            }
            if(!_colorRules.isEmpty()) {
                saveColorRules(writer);
            }
            writer.writeEndElement();
        }

        void AnnoProject::loadClassPath(QXmlStreamReader &reader)
        throw(XmlException *) {
            QString tagList(XML_CLASSPATH);
            QString tagFile("file");

            if (!reader.isStartElement() || reader.name() != tagList) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML. Was expecting " + XML_CLASSPATH);
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
            QString tagList(XML_SEARCHPATH);
            QString tagDir("dir");

            if (!reader.isStartElement() || reader.name() != tagList) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML. Was expecting " + XML_SEARCHPATH);
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
            QString tagList(XML_LINK);
            QString tagUuid("uuid");

            if (!reader.isStartElement() || reader.name() != tagList) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid XML. Was expecting " + XML_LINK);
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

        void AnnoProject::loadFilters(QXmlStreamReader &reader) throw(XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || curParent != XML_FILTERS) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_FILTERS, curParent);
            }

            XmlHelper::skipToNextStartElement(true, reader);
            while(!reader.atEnd()) {
                if(reader.isStartElement() && reader.name() == filter::AnnoFilter::XML_SINGLEFILTER) {
                    filter::AnnoFilter *pFilter = filter::AnnoFilter::fromXml(reader);
                    if(pFilter == NULL) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, QString("Encountered unknown Filter Tag <%1>").arg(reader.name().toString()));
                    }
                    if(_filters.contains(pFilter->getName())) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, QString("Double filter name encountered <%1>").arg(pFilter->getName()));
                    }
                    _filters.insert(pFilter->getName(), pFilter);

                    continue;
                } else if(reader.isEndElement() && reader.name().toString() == XML_FILTERS) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }

        void AnnoProject::saveFilters(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement(XML_FILTERS);
            foreach(filter::AnnoFilter *f, _filters.values()) {
                f->toXml(writer);
            }
            writer.writeEndElement();
        }

        void AnnoProject::loadColorRules(QXmlStreamReader &reader) throw(XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || curParent != XML_COLORRULES) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_COLORRULES, curParent);
            }

            XmlHelper::skipToNextStartElement(true, reader);
            while(!reader.atEnd()) {
                if(reader.isStartElement() && reader.name() == filter::ColorFilterEntry::XML_SINGLECOLORRULE) {
                    filter::ColorFilterEntry *pColor = filter::ColorFilterEntry::fromXml(reader);
                    if(pColor == NULL) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, QString("Encountered unknown Color Rule Tag<%1>").arg(reader.name().toString()));
                    }
                    _colorRules.append(pColor);

                    continue;
                } else if(reader.isEndElement() && reader.name().toString() == XML_COLORRULES) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }

        void AnnoProject::saveColorRules(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement(XML_COLORRULES);
            foreach(filter::ColorFilterEntry *c, _colorRules) {
                c->toXml(writer);
            }
            writer.writeEndElement();
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
