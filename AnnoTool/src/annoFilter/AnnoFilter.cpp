#include "include/AnnoFilter.h"
#include "Annotation.h"
#include "AnnoFileData.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace anno {
    namespace filter {
        AnnoFilter::AnnoFilter(QObject *parent) :
            QObject(parent), _active(false), _rule(NULL), _fileData(NULL) {
        }

        AnnoFilter::AnnoFilter(AnnoFilterRule *rule, dt::AnnoFileData *fileData,
                               QObject *parent) :
            QObject(parent), _active(false), _rule(rule), _fileData(fileData) {
        }

        AnnoFilter::AnnoFilter(const QString &name, AnnoFilterRule *rule, dt::AnnoFileData *fileData, QObject *parent) :
            QObject(parent), _active(false), _name(name), _rule(rule), _fileData(fileData) {
        }

        AnnoFilter::AnnoFilter(const QString &name, const QString &comment, AnnoFilterRule *rule, dt::AnnoFileData *fileData, QObject *parent) :
            QObject(parent), _active(false), _name(name), _comment(comment), _rule(rule), _fileData(fileData) {
        }

        AnnoFilter::~AnnoFilter() {
            if(_rule != NULL) {
                delete _rule;
            }
        }

        void AnnoFilter::invalidate() {
            if(_active) {
                disconnectSignals();
                _active = false;
                _filteredAnnoMap.clear();
            }
        }
        void AnnoFilter::deactivate() {
            if(_active) {
                disconnectSignals();
                _active = false;
            }
        }

        bool AnnoFilter::connectSignals() {
            if(_fileData == NULL) {
                return false;
            }

            bool conOk = true;
            conOk = conOk && connect(_fileData, SIGNAL(annoAdded(::anno::dt::Annotation *)), this, SLOT(onAnnoAdded(::anno::dt::Annotation *)));
            conOk = conOk && connect(_fileData, SIGNAL(annoRemoved(QUuid)), this, SLOT(onAnnoRemoved(QUuid)));
            conOk = conOk && connect(_fileData, SIGNAL(modified(::anno::dt::AnnoFileData *)), this, SLOT(onAnnoUpdated(::anno::dt::Annotation *)));
            return conOk;
        }

        bool AnnoFilter::disconnectSignals() {
            if(_fileData == NULL) {
                return false;
            }

            bool conOk = true;
            conOk = conOk && disconnect(_fileData, SIGNAL(annoAdded(::anno::dt::Annotation *)), this, SLOT(onAnnoAdded(::anno::dt::Annotation *)));
            conOk = conOk && disconnect(_fileData, SIGNAL(annoRemoved(QUuid)), this, SLOT(onAnnoRemoved(QUuid)));
            conOk = conOk && disconnect(_fileData, SIGNAL(modified(::anno::dt::AnnoFileData *)), this, SLOT(onAnnoUpdated(::anno::dt::Annotation *)));
            return conOk;
        }

        void AnnoFilter::onAnnoAdded(anno::dt::Annotation *anno) {
            if(_active && _rule != NULL) {
                if(_rule->eval(anno)) {
                    _filteredAnnoMap.insert(anno->annoId(), anno);
                    emit filterAnnoAdded(anno);
                    emit filterSetChanged(true);
                }
            }
        }

        void AnnoFilter::onAnnoRemoved(QUuid uuid) {
            if(_active && _rule != NULL) {
                if(_filteredAnnoMap.contains(uuid)) {
                    _filteredAnnoMap.remove(uuid);
                    emit filterAnnoRemoved(uuid);
                    emit filterSetChanged(true);
                }
            }
        }

        void AnnoFilter::onAnnoUpdated(anno::dt::Annotation *anno) {
            if (_active && _rule != NULL) {
                if (_rule->eval(anno)) {
                    if(!_filteredAnnoMap.contains(anno->annoId())) {
                        _filteredAnnoMap.insert(anno->annoId(), anno);
                        emit filterAnnoAdded(anno);
                        emit filterSetChanged(true);
                    }
                } else {
                    if(_filteredAnnoMap.remove(anno->annoId()) > 0) {
                        emit filterAnnoRemoved(anno->annoId());
                        emit filterSetChanged(true);
                    }
                }
            }
        }

        bool AnnoFilter::applyFilter(bool partial) {
            if(_rule == NULL || _fileData == NULL) {
                return false;
            }

            if(partial) {
                _active = false;
                QList<dt::Annotation *> annoList = _filteredAnnoMap.values();
                _filteredAnnoMap.clear();
                QListIterator<dt::Annotation *> it = QListIterator<dt::Annotation *>(annoList);
                bool changed = false;
                try {
                    while(it.hasNext()) {
                        dt::Annotation *curAnno = it.next();
                        if(_rule->eval(curAnno)) {
                            _filteredAnnoMap.insert(curAnno->annoId(), curAnno);
                            changed = true;
                        }
                    }
                } catch(exc::AnnoException *e) {
                    delete e;
                    _filteredAnnoMap.clear();
                    return false;
                }
                _active = true;
                connectSignals();
                if(changed) {
                    emit filterSetChanged(false);
                }
                return true;
            } else {
                _active = false;
                _filteredAnnoMap.clear();
                QListIterator<dt::Annotation *> it = _fileData->getAnnoIterator();
                bool changed = false;
                try {
                    while(it.hasNext()) {
                        dt::Annotation *curAnno = it.next();
                        if(_rule->eval(curAnno)) {
                            _filteredAnnoMap.insert(curAnno->annoId(), curAnno);
                            changed = true;
                        }
                    }
                } catch(exc::AnnoException *e) {
                    delete e;
                    _filteredAnnoMap.clear();
                    return false;
                }
                _active = true;
                connectSignals();
                if(changed) {
                    emit filterSetChanged(false);
                }
                return true;
            }
        }

        void AnnoFilter::resetFilter() {
            _filteredAnnoMap.clear();
            if(_active) {
                disconnectSignals();
                _active = false;
            }
            _fileData = NULL;
        }

        int AnnoFilter::filterCount() const {
            return _filteredAnnoMap.size();
        }

        dt::Annotation *AnnoFilter::getAnnotation(const QUuid &uuid) {
            return _filteredAnnoMap.value(uuid, NULL);
        }

        const dt::Annotation *AnnoFilter::getAnnotation(const QUuid &uuid) const {
            return _filteredAnnoMap.value(uuid, NULL);
        }

        bool AnnoFilter::containsAnnotation(const QUuid &uuid) const {
            return _filteredAnnoMap.contains(uuid);
        }

        bool AnnoFilter::containsAnnotation(const dt::Annotation *anno) const {
            return _filteredAnnoMap.contains(anno->annoId());
        }

        QList<dt::Annotation *> AnnoFilter::getAnnoList() const {
            return _filteredAnnoMap.values();
        }

        //TODO finish this and implement deep copy on rules.
        AnnoFilter *AnnoFilter::getCleanCopy(QObject *parent) const {
            AnnoFilter *filter = new AnnoFilter(parent);
            filter->_name = _name;
            filter->_comment = _comment;
            filter->_rule = _rule;

            return filter;
        }

        void AnnoFilter::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeStartElement("annoFilter");
            writer.writeAttribute("name", _name);
            if(!_comment.isEmpty()) {
                writer.writeTextElement("comment", _comment);
            }
            if(_rule != NULL) {
                writer.writeStartElement("filterRule");
                _rule->toXml(writer);
                writer.writeEndElement();
            }
            writer.writeEndElement();
        }

        void AnnoFilter::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            //TODO implement this!
        }

        AnnoFilter *AnnoFilter::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            //TODO implement this!
            return NULL;
        }


    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
