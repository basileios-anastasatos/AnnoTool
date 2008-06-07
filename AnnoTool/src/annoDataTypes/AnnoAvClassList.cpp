#include "include/AnnoAvClassList.h"
#include "XmlHelper.h"
#include <QFile>
#include <QMap>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>

namespace anno {

    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoAvClassList::AnnoAvClassList() {
        }

        AnnoAvClassList::~AnnoAvClassList() {
            if (!_classes.isEmpty()) {
                QListIterator<AnnoClassDefinition *> i(_classes);
                while (i.hasNext()) {
                    delete i.next();
                }
            }
        }

        bool AnnoAvClassList::contains(const QString &name) const {
            QListIterator<AnnoClassDefinition *> i(_classes);
            while (i.hasNext()) {
                if (i.next()->name() == name) {
                    return true;
                }
            }
            return false;
        }

        void AnnoAvClassList::add(AnnoClassDefinition *anClass)
        throw(NameConflictException *) {
            if (contains(anClass->name())) {
                throw new NameConflictException(__FILE__, __LINE__, QString("Given annotation class [%1] does already exist.").arg(anClass->name()));
            }

            _classes.append(anClass);
        }

        void AnnoAvClassList::remove(const QString &name) {
            for (int i = 0; i < _classes.size(); ++i) {
                if (_classes[i]->name() == name) {
                    delete _classes[i];
                    _classes.removeAt(i);
                }
            }
        }

        AnnoClassDefinition *AnnoAvClassList::getClass(const QString &name) const {
            for (int i = 0; i < _classes.size(); ++i) {
                if (_classes[i]->name() == name) {
                    return _classes[i];
                }
            }

            return NULL;
        }

        AnnoClassDefinition *AnnoAvClassList::getClass(int index) const {
            if (index < 0 || index >= _classes.size()) {
                return NULL;
            }

            return _classes[index];
        }

        QList<QString> AnnoAvClassList::getClassNames() const {
            QList<QString> names;
            QListIterator<AnnoClassDefinition *> i(_classes);
            while (i.hasNext()) {
                names.append(i.next()->name());
            }
            return names;
        }

        int AnnoAvClassList::classCount() const {
            return _classes.size();
        }

        void AnnoAvClassList::addFromFile(const QString &path) throw(IOException *, XmlException *) {
            QTextStream out(stdout);
            out << "Loading from file " << path << endl;
            QFile file(path);
            if (!file.exists()) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot load from [%1]. File does not exist.").arg(path));
            } else if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot load from [%1]. File cannot be opened.").arg(path));
            }

            out << "File opened" << endl;
            QXmlStreamReader reader(&file);
            reader.setNamespaceProcessing(true);
            if (!XmlHelper::skipToStartElement("annoClassDefinitions", reader)) {
                throw new XmlFormatException(__FILE__, __LINE__, QString("Cannot load from [%1]. Invalid XML format.").arg(path));
            }

            out << "Starting class reading..." << endl;
            addFromXml(reader);
            file.close();
        }

        void AnnoAvClassList::addFromXml(QXmlStreamReader &reader)
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
                }
                //TODO think about ignoring dublicate class defs.
                else if (contains(name)) {
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

        void AnnoAvClassList::print() const {
            QTextStream out(stdout);
            out << "Classes: " << _classes.size() << endl;
            out << "--------------------------------------------------------" << endl;
            QListIterator<AnnoClassDefinition *> i(_classes);
            while (i.hasNext()) {
                i.next()->print();
            }
            out << "--------------------------------------------------------" << endl;
        }

    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
