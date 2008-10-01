#include "include/AnnoFileData.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QTextStream>

#include "importGlobals.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoFileData::~AnnoFileData() {
            if (!_annoList.isEmpty()) {
                QListIterator<Annotation *> i(_annoList);
                while (i.hasNext()) {
                    delete i.next();
                }
            }
        }

        void AnnoFileData::onAnnoInfoNotify(bool prevState, bool curState) {
            if (curState) {
                setModified(true);
            }
        }

        void AnnoFileData::onAnnoImageInfoNotify(bool prevState, bool curState) {
            if (curState) {
                setModified(true);
            }
        }

        void AnnoFileData::onAnnoModified(::anno::dt::Annotation *anno) {
            if (anno != NULL) {
                if (_notifyAnno) {
                    emit annoModified(this, anno);
                }
                if (anno->isModified()) {
                    setModified(true);
                }
            }
        }

        void AnnoFileData::onAnnoModifyReset(::anno::dt::Annotation *anno) {
            if (anno != NULL) {
                if (_notifyAnno) {
                    emit annoModifyReset(this, anno);
                }
            }
        }

        void AnnoFileData::onAnnoModifyStateChanged(::anno::dt::Annotation *anno, bool prevState,
                bool curState) {
            if (anno != NULL) {
                if (_notifyOnChangeAnno) {
                    emit annoModifyStateChanged(this, anno, prevState, curState);
                }
            }
        }

        void AnnoFileData::setModified(bool mod) {
            bool tmp = _modified;
            _modified = mod;

            if (_notify) {
                if (mod) {
                    emit modified(this);
                } else {
                    emit modifyReset(this);
                }
            }
            if (_notifyOnChange && _modified != tmp) {
                emit modifyStateChanged(this, tmp, mod);
            }
        }

        void AnnoFileData::resetModifiedState(bool noNotify) {
            bool tmpNotify = _notify;
            bool tmpNotifyOnChange = _notifyOnChange;
            bool tmpNotifyAnno = _notifyAnno;
            bool tmpNotifyOnChangeAnno = _notifyOnChangeAnno;

            if (noNotify) {
                _notify = false;
                _notifyOnChange = false;
                _notifyAnno = false;
                _notifyOnChangeAnno = false;
            }

            QMutableListIterator<Annotation *> it(_annoList);
            while (it.hasNext()) {
                it.next()->resetModifiedState(noNotify);
            }
            _imageInfo.setModified(false);
            _annoInfo.setModified(false);
            setModified(false);

            if (noNotify) {
                _notify = tmpNotify;
                _notifyOnChange = tmpNotifyOnChange;
                _notifyAnno = tmpNotifyAnno;
                _notifyOnChangeAnno = tmpNotifyOnChangeAnno;
            }
        }

        int AnnoFileData::annoParentCount() {
            int count = 0;
            QListIterator<Annotation *> iterator(_annoList);
            while(iterator.hasNext()) {
                if(!iterator.next()->hasAnnoParent()) {
                    ++count;
                }
            }
            return count;
        }

        QList<QUuid> AnnoFileData::annoParents() {
            QList<QUuid> parents;
            QListIterator<Annotation *> iterator(_annoList);
            while(iterator.hasNext()) {
                Annotation *cur = iterator.next();
                if(!cur->hasAnnoParent()) {
                    parents.append(cur->annoId());
                }
            }
            return parents;
        }

        QList<const Annotation *> AnnoFileData::annoList() const {
            QList<const Annotation *> lst;
            QListIterator<Annotation *> i(_annoList);
            while (i.hasNext()) {
                lst.append(i.next());
            }
            return lst;
        }

        void AnnoFileData::addAnnotation(Annotation *anno) {
            QUuid uuid = anno->annoId();
            if (!_annoMap.contains(uuid)) {
                anno->setParent(this);
                anno->setNotify(true);
                anno->setNotifyOnChange(_notifyOnChangeAnno);
                anno->setNotifyAttr(false);
                bool conOk = true;
                conOk = conOk && connect(anno, SIGNAL(modified(::anno::dt::Annotation *)), this, SLOT(onAnnoModified(::anno::dt::Annotation *)));
                conOk = conOk && connect(anno, SIGNAL(modifyReset(::anno::dt::Annotation *)), this, SLOT(onAnnoModifyReset(::anno::dt::Annotation *)));
                conOk = conOk && connect(anno, SIGNAL(modifyStateChanged(::anno::dt::Annotation *, bool, bool)), this, SLOT(onAnnoModifyStateChanged(::anno::dt::Annotation *, bool, bool)));
                _annoList.append(anno);
                _annoMap.insert(uuid, anno);
                setModified(true);
                emit annoAdded(anno);

                if(!conOk) {
                    GlobalLogger::instance()->logError("CONNECT-ERROR: AnnoFileData::addAnnotation");
                }
            }
        }

        void AnnoFileData::removeAnnotation(int index) {
            if (index >= 0 && index < _annoList.size()) {
                Annotation *anno = _annoList.at(index);
                QUuid annoUuid = anno->annoId();
                _annoMap.remove(anno->annoId());
                _annoList.removeAt(index);
                delete anno;
                setModified(true);
                emit annoRemoved(annoUuid);
            }
        }

        void AnnoFileData::removeAnnotation(const QUuid &uuid) {
            Annotation *anno = _annoMap.value(uuid, NULL);
            if (anno != NULL) {
                int index = _annoList.indexOf(anno, 0);
                if (index >= 0) {
                    _annoMap.remove(uuid);
                    _annoList.removeAt(index);
                    delete anno;
                    setModified(true);
                    emit annoRemoved(uuid);
                }
            }
        }

        int AnnoFileData::getAnnotationIndex(const QUuid &uuid) const {
            if(!containsAnnotation(uuid)) {
                return -1;
            }

            for(int i = 0; i < _annoList.size(); ++i) {
                if(_annoList[i]->annoId() == uuid) {
                    return i;
                }
            }
            return -1;
        }

        int AnnoFileData::getAnnotationIndex(const Annotation *anno) const {
            if (anno == NULL || !containsAnnotation(anno->annoId())) {
                return -1;
            }

            QUuid uuid = anno->annoId();
            for (int i = 0; i < _annoList.size(); ++i) {
                if (_annoList[i]->annoId() == uuid) {
                    return i;
                }
            }
            return -1;
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

        void AnnoFileData::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != "imageAnnotationFile") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "imageAnnotationFile", reader.name().toString());
            }

            if (!XmlHelper::skipToStartElement("imageInfo", reader)) {
                throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "imageInfo", reader.name().toString());
            }
            _imageInfo = AnnoImageInfo::fromXml(reader);
            _imageInfo.setParentFile(this);

            if (!XmlHelper::skipToStartElement("annotationInfo", reader)) {
                throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "annotationInfo", reader.name().toString());
            }
            _annoInfo = AnnoInfo::fromXml(reader);
            _annoInfo.setParentFile(this);

            if (!XmlHelper::skipToStartElement("imageAnnotations", reader)) {
                throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "imageAnnotations", reader.name().toString());
            }

            QString tagAnnoLst("imageAnnotations");
            QString tagAnno("annotation");
            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagAnno) {
                    addAnnotation(Annotation::fromXml(reader));
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

        AnnoFileData *AnnoFileData::fromFile(const QString &path) throw(IOException *,
                XmlException *) {
            AnnoFileData *data = new AnnoFileData(path);
            data->loadFromFile();
            return data;
        }

        void AnnoFileData::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
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

        AnnoFileData *AnnoFileData::fromXml(QXmlStreamReader &reader) throw(XmlException *) {
            AnnoFileData *data = new AnnoFileData("unknown");
            data->setAllNotifications(false);
            data->loadFromXml(reader);
            data->_sourceFile = data->imageInfo()->imageIdAsString();
            return data;
        }

        bool AnnoFileData::probeFile(const QString &file, const QUuid &uuid)
        throw(IOException *) {
            QFile f(file);
            if (!f.exists()) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot open [%1]. File does not exist.").arg(file));
            } else if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                throw new IOException(__FILE__, __LINE__, QString("Cannot open [%1]. File cannot be opened.").arg(file));
            }

            QXmlStreamReader reader(&f);
            reader.setNamespaceProcessing(true);
            if (!XmlHelper::skipToStartElement("imageAnnotationFile", reader)) {
                return false;
            }
            if (!XmlHelper::skipToStartElement("annotationInfo", reader)) {
                return false;
            }

            QString tag("annotationComplex");
            QString tagEnd("annotationInfo");
            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tag) {
                    QString val = reader.readElementText();
                    if (uuid == QUuid(val)) {
                        return true;
                    }
                } else if (reader.isEndElement() && reader.name() == tagEnd) {
                    break;
                } else {
                    reader.readNext();
                }
            }
            return false;
        }

    } //end namespace dt
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
