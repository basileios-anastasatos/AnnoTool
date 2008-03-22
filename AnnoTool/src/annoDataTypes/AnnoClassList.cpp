#include "include/AnnoClassList.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <iostream>
#include <QTextStream>
#include <QMap>

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {

        AnnoClassList::AnnoClassList(QString file) {
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

        bool AnnoClassList::contains(QString name) const {
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

        void AnnoClassList::remove(QString name) {
            for (int i = 0; i < _classes.size(); ++i) {
                if (_classes[i]->name() == name) {
                    delete _classes[i];
                    _classes.removeAt(i);
                }
            }
        }

        AnnoClassDefinition *AnnoClassList::getClass(QString name) const
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
            if(index < 0 || index >= _classes.size()) {
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

        bool AnnoClassList::skipTo(QString tagName, QXmlStreamReader &reader)
        throw(XmlException *) {
            bool found = false;
            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagName) {
                    found = true;
                    break;
                }
                reader.readNext();
            }

            if (reader.hasError()) {
                throw new XmlException(__FILE__, __LINE__, QString("An error occured while processing XML stream: %1").arg(reader.errorString()));
            }

            return found;
        }

        void AnnoClassList::loadFromFile() throw(IOException *,
                XmlException *) {
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
            if (!skipTo("annoClassDefinitions", reader)) {
                throw new XmlFormatException(__FILE__, __LINE__, QString("Cannot load from [%1]. Invalid XML format.").arg(_sourceFile));
            }

            out << "Starting class reading..." << endl;
            QString classTag("classDef");
            QString attrName("id");
            QString attrParent("extends");
            QMap<QString, QString> parentMap;
            while (skipTo(classTag, reader)) {
                QString name = reader.attributes().value(attrName).toString();
                out << "Reading class: " << name << endl;
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
                out << "Finished reading class" << endl;
                _classes.append(cl);
            }

            QMapIterator<QString, QString> i(parentMap);
            while (i.hasNext()) {
                i.next();
                getClass(i.key())->setParent(getClass(i.value()));
            }
        }

        void AnnoClassList::writeToFile() const throw(IOException *) {
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
            writer.writeStartElement("annoClassDefinitions");
            QListIterator<AnnoClassDefinition *> i(_classes);
            while (i.hasNext()) {
                i.next()->toXml(writer);
            }
            writer.writeEndElement();
            writer.writeEndDocument();
            file.flush();
            file.close();
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
