#include "include/AnnoFilterManager.h"

namespace anno {
    namespace filter {

        AnnoFilterManager::AnnoFilterManager(QObject *parent) :
            QObject(parent), _enabled(false), _curFilter(NULL) {
        }

        AnnoFilterManager::~AnnoFilterManager() {
            if(!_filters.isEmpty()) {
                qDeleteAll(_filters);
            }
        }

        bool AnnoFilterManager::connectFilterSignals() {
            if(_curFilter == NULL) {
                return false;
            }

            bool conOk = true;
            conOk = conOk && connect(_curFilter, SIGNAL(filterSetChanged(bool)), this, SIGNAL(curFilterSetChanged(bool)));
            conOk = conOk && connect(_curFilter, SIGNAL(filterAnnoAdded(::anno::dt::Annotation *)), this, SIGNAL(curFilterAnnoAdded(::anno::dt::Annotation *)));
            conOk = conOk && connect(_curFilter, SIGNAL(filterAnnoRemoved(QUuid)), this, SIGNAL(curFilterAnnoRemoved(QUuid)));
            return conOk;
        }

        bool AnnoFilterManager::disconnectFilterSignals() {
            if(_curFilter == NULL) {
                return false;
            }

            bool conOk = true;
            conOk = conOk && disconnect(_curFilter, SIGNAL(filterSetChanged(bool)), this, SIGNAL(curFilterSetChanged(bool)));
            conOk = conOk && disconnect(_curFilter, SIGNAL(filterAnnoAdded(::anno::dt::Annotation *)), this, SIGNAL(curFilterAnnoAdded(::anno::dt::Annotation *)));
            conOk = conOk && disconnect(_curFilter, SIGNAL(filterAnnoRemoved(QUuid)), this, SIGNAL(curFilterAnnoRemoved(QUuid)));
            return conOk;
        }


        void AnnoFilterManager::setEnabled(bool enable) {
            _enabled = enable;
            if(_curFilter != NULL) {
                if(!_enabled) {
                    _curFilter->deactivate();
                } else {
                    _curFilter->applyFilter();
                }
            }
        }

        void AnnoFilterManager::selectFilter(const QString &filterName) {
            if((_curFilter != NULL && _curFilter->getName() == filterName) ||
                    (_curFilter == NULL && filterName.isEmpty())) {
                return;
            }

            if(filterName.isEmpty()) {
                if(_curFilter != NULL) {
                    disconnectFilterSignals();
                }
                _curFilter = NULL;
                emit filterSelected(filterName);
            } else if(_filters.contains(filterName)) {
                if(_curFilter != NULL) {
                    disconnectFilterSignals();
                }
                _curFilter = _filters.value(filterName);
                connectFilterSignals();
                emit filterSelected(filterName);
            }
        }

        void AnnoFilterManager::applyFilter() {
            if(_curFilter != NULL) {
                _curFilter->applyFilter();
            }
        }

        int AnnoFilterManager::filterCount() const {
            return  _filters.size();
        }

        AnnoFilter *AnnoFilterManager::getFilter(const QString &filterName) {
            return _filters.value(filterName, NULL);
        }

        QList<AnnoFilter *> AnnoFilterManager::getAllFilters() const {
            return _filters.values();
        }

        AnnoFilter *AnnoFilterManager::curFilter() {
            return _curFilter;
        }

        bool AnnoFilterManager::containsFilter(const QString &filterName) const {
            return _filters.contains(filterName);
        }

        bool AnnoFilterManager::containsFilter(AnnoFilter *filter) const {
            return _filters.contains(filter->getName());
        }

        bool AnnoFilterManager::addFilter(AnnoFilter *filter) {
            if(_filters.contains(filter->getName())) {
                return false;
            }

            _filters.insert(filter->getName(), filter);
            emit filterAdded(filter);
            return true;
        }

        bool AnnoFilterManager::removeFilter(const QString &filterName, bool delFilter) {
            AnnoFilter *filter = _filters.value(filterName, NULL);
            if(filter != NULL && _filters.remove(filterName) > 0) {
                if(delFilter) {
                    delete filter;
                }
                emit filterRemoved(filterName);
                return true;
            }
            return false;
        }

    } //end: namespace filter
} //end: namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
