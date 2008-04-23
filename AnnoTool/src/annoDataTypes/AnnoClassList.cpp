#include "include/AnnoClassList.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QMap>
#include "XmlHelper.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoClassList::AnnoClassList(const QString &file) {
            _sourceFile = file;
        }

        AnnoClassList::~AnnoClassList() {
            if (!_classes.isEmpty()) {
                QListIterator<AnnoClassDefinition *> i(_classes);
                while (i.hasNext()) {
                    delete i.next();
                }
            }
        }

        bool AnnoClassList::contains(const QString &name) const {
            QListIterator<AnnoClassDefinition *> i(_classes);
            while (i.hasNext()) {
                if (i.next()->name() == name) {
                    return true;
                }
            }
            return false;
        }

        void AnnoClassList::add(AnnoClassDefinition *anClass)
        throw(NameConflictException *) {
            if (contains(anClass->name())) {
                throw new NameConflictException(__FILE__, __LINE__, QString("Given annotation class [%1] does already exist.").arg(anClass->name()));
            }

            _classes.append(anClass);
        }

        void AnnoClassList::remove(const QString &name) {
            for (int i = 0; i < _classes.size(); ++i) {
                if (_classes[i]->name() == name) {
                    delete _classes[i];
                    _classes.removeAt(i);
                }
            }
        }

        AnnoClassDefinition *AnnoClassList::getClass(const QString &name) const
        throw(NoSuchElementException *) {
            for (int i = 0; i < _classes.size(); ++i) {
                if (_classes[i]->name() == name) {
                    return _classes[i];
                }
            }

            throw new NoSuchElementException(__FILE__, __LINE__, QString("Given annotation class [%1] cannot be found.").arg(name));
        }

        AnnoClassDefinition *AnnoClassList::getClass(int index) const
        throw(OutOfRangeException *) {
            if (index < 0 || index >= _classes.size()) {
                throw new OutOfRangeException(__FILE__, __LINE__, "Given index is out of list bounds.");
            }

            return _classes[index];
        }

        QList<QString> AnnoClassList::getClassNames() const {
            QList<QString> names;
            QListIterator<AnnoClassDefinition *> i(_classes);
            while (i.hasNext()) {
                names.append(i.next()->name());
            }
            return names;
        }

        int AnnoClassList::classCount() const {
            return _classes.size();
        }

        QString AnnoClassList::filePath() const {
            return _sourceFile;
        }

        void AnnoClassList::setFilePath(const QString &path) {
            _sourceFile = path;
        }

        void AnnoClassList::loadFromFile() throw(IOException *, XmlException *) {
            QTextStream out(stdout);
            out << "Loading from file " << _sourceFile << endl;
            QFile file(_sourceFile);
            if (!file.exists()) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot load from [%1]. File does not exist.").arg(_sourceFile));
            } else if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot load from [%1]. File cannot be opened.").arg(_sourceFile));
            }

            out << "File opened" << endl;
            QXmlStreamReader reader(&file);
            reader.setNamespaceProcessing(true);
            if (!XmlHelper::skipToStartElement("annoClassDefinitions", reader)) {
                throw new XmlFormatException(__FILE__, __LINE__, QString("Cannot load from [%1]. Invalid XML format.").arg(_sourceFile));
            }

            out << "Starting class reading..." << endl;
            loadFromXml(reader);
            file.close();
        }

        void AnnoClassList::loadFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != "annoClassDefinitions") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "annoClassDefinitions", reader.name().toString());
            }

            QString classTag("classDef");
            QString attrName("id");
            QString attrParent("extends");
            QMap<QString, QString> parentMap;
            while (XmlHelper::skipToStartElement(classTag, reader)) {
                QString name = reader.attributes().value(attrName).toString();
                if (name.isEmpty()) {
                    throw new XmlFormatException(__FILE__, __LINE__, "Class id must not be empty.");
                } else if (contains(name)) {
                    throw new XmlException(__FILE__, __LINE__, QString("Class id must be unique [%1].").arg(name));
                }

                QString parent = reader.attributes().value(attrParent).toString();
                if (!parent.isEmpty()) {
                    parentMap.insert(name, parent);
                }

                AnnoClassDefinition *cl = new AnnoClassDefinition(name);
                cl->attributesFromXml(reader);
                _classes.append(cl);
            }

            QMapIterator<QString, QString> i(parentMap);
            while (i.hasNext()) {
                i.next();
                getClass(i.key())->setParent(getClass(i.value()));
            }
        }

        void AnnoClassList::writeToFile() const throw(IOException *, XmlException *) {
            QTextStream out(stdout);
            out << "Writing to file " << _sourceFile << endl;
            QFile file(_sourceFile);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot write to [%1]. File cannot be opened.").arg(_sourceFile));
            }

            out << "File opened" << endl;
            QXmlStreamWriter writer(&file);
            writer.setAutoFormatting(true);
            writer.writeStartDocument();
            toXml(writer);
            writer.writeEndDocument();
            file.flush();
            file.close();
        }

        AnnoClassList *AnnoClassList::fromFile(const QString &path)
        throw(IOException *, XmlException *) {
            AnnoClassList *data = new AnnoClassList(path);
            data->loadFromFile();
            return data;
        }

        void AnnoClassList::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("annoClassDefinitions");
            QListIterator<AnnoClassDefinition *> i(_classes);
            while (i.hasNext()) {
                i.next()->toXml(writer);
            }
            writer.writeEndElement();
        }

        AnnoClassList *AnnoClassList::fromXml(QXmlStreamReader &reader)
        throw(IOException *, XmlException *) {
            AnnoClassList *data = new AnnoClassList("unknown");
            data->loadFromXml(reader);
            return data;
        }

        void AnnoClassList::print() const {
            QTextStream out(stdout);
            out << "AnnoClassList from" << endl << _sourceFile << endl;
            out << "Classes: " << _classes.size() << endl;
            out << "--------------------------------------------------------" << endl;
            QListIterator<AnnoClassDefinition *> i(_classes);
            while (i.hasNext()) {
                i.next()->print();
            }
            out << "--------------------------------------------------------" << endl;
        }

    } //end namespace dt
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
