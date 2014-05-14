#include "include/AnnoFilterManager.h"
#include "AfrAnd.h"
#include "AfrSwitch.h"
#include "AfrScoreAdv.h"
#include "importGlobals.h"
#include "AnnoProject.h"
#include "ColorFilterEntry.h"

namespace anno {
    namespace filter {

        AnnoFilterManager::AnnoFilterManager(dt::AnnoProject *project, QObject *parent) :
            QObject(parent),
            _commonEnabled(false),
            _scoreEnabled(false),
            _filters(NULL),
            _commonFilter(NULL),
            _emptyFilter(NULL),
            _scoreFilter(NULL),
            _unifiedFilter(NULL),
            _curFilter(NULL),
            _scoreLink(NULL),
            _scoreRule(NULL),
            _coloringEnabled(false),
            _colorRules(NULL) {
            if (project) {
                _filters = project->filters();
                _colorRules = project->colorRules();
            }
            _emptyFilter = new AnnoFilter("#_Auto_Empty_Filter_#", "Automatic generated empty filter.", new AfrSwitch(true));
            _scoreLink = new AfrAnd(false);
            _scoreRule = new AfrScoreAdv(0.0, 10000.0, true, true, false);
            _scoreRule->setConstValue(true);
            _scoreLink->addChild(_scoreRule);
            _scoreFilter = new AnnoFilter("#_Auto_Score_Filter_#", "Automatic generated advanced score filter.", _scoreRule);
            _unifiedFilter = new AnnoFilter("#_Auto_Unified_Filter_#", "Automatic generated unified filter.", _scoreLink);

            connect(GlobalProjectManager::instance(), SIGNAL(curAnnoFileSelChanged(int, QUuid, ::anno::dt::AnnoFileData *)), this, SLOT(onPM_annoFileSelectChanged(int, QUuid, ::anno::dt::AnnoFileData *)));
        }

        void AnnoFilterManager::setProject(dt::AnnoProject *project) throw (anno::exc::AnnoException) {
            if (!project) {
                throw new anno::exc::AnnoException(__FILE__, __LINE__, "Invalid argument to AnnoFilterManager::setProject()");
            }
            if (!_filters) {
                _filters = project->filters();
            }
            if (!_colorRules) {
                _colorRules = project->colorRules();
            }
        }

        AnnoFilterManager::~AnnoFilterManager() {
            if(_emptyFilter != NULL) {
                delete _emptyFilter;
            }
            if(_scoreFilter != NULL) {
                delete _scoreFilter;
            }
            if(_unifiedFilter != NULL) {
                delete _unifiedFilter;
            }
        }

        bool AnnoFilterManager::connectFilterSignals() {
            if(_curFilter == NULL) {
                return false;
            }

            bool conOk = true;
            conOk = conOk && connect(_curFilter, SIGNAL(filterBegin(int)), this, SIGNAL(curFilterBegin(int)));
            conOk = conOk && connect(_curFilter, SIGNAL(filterEnd(int, int)), this, SIGNAL(curFilterEnd(int, int)));
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
            conOk = conOk && disconnect(_curFilter, SIGNAL(filterBegin(int)), this, SIGNAL(curFilterBegin(int)));
            conOk = conOk && disconnect(_curFilter, SIGNAL(filterEnd(int, int)), this, SIGNAL(curFilterEnd(int, int)));
            conOk = conOk && disconnect(_curFilter, SIGNAL(filterSetChanged(bool)), this, SIGNAL(curFilterSetChanged(bool)));
            conOk = conOk && disconnect(_curFilter, SIGNAL(filterAnnoAdded(::anno::dt::Annotation *)), this, SIGNAL(curFilterAnnoAdded(::anno::dt::Annotation *)));
            conOk = conOk && disconnect(_curFilter, SIGNAL(filterAnnoRemoved(QUuid)), this, SIGNAL(curFilterAnnoRemoved(QUuid)));
            return conOk;
        }

        void AnnoFilterManager::rebuildCurrentFilter() {
            if(_curFilter != NULL) {
                disconnectFilterSignals();
            }

            if(_commonEnabled) {
                if(_scoreEnabled) {
                    if(_scoreLink->childCount() > 1) {
                        _scoreLink->removeChild(1);
                    }
                    _scoreLink->addChild(_commonFilter->getFilterRule());
                    _curFilter = _unifiedFilter;
                    connectFilterSignals();
                } else {
                    _curFilter = _commonFilter;
                    connectFilterSignals();
                }
            } else if(_scoreEnabled) {
                _curFilter = _scoreFilter;
                connectFilterSignals();
            } else {
                _curFilter = _emptyFilter;
            }
        }

        AnnoFilter *AnnoFilterManager::getScoreFilter() {
            return _scoreFilter;
        }

        void AnnoFilterManager::setScoreLower(const double &value) {
            if(_scoreRule != NULL && _scoreFilter != NULL) {
                _scoreRule->setLower(value);
                if(_scoreEnabled) {
                    applyFilter();
                }
            }
        }

        void AnnoFilterManager::setScoreUpper(const double &value) {
            if(_scoreRule != NULL && _scoreFilter != NULL) {
                _scoreRule->setUpper(value);
                if(_scoreEnabled) {
                    applyFilter();
                }
            }
        }

        double AnnoFilterManager::getScoreLower() const {
            if(_scoreRule != NULL) {
                return _scoreRule->getLower();
            }
            return -42.4242;
        }

        double AnnoFilterManager::getScoreUpper() const {
            if(_scoreRule != NULL) {
                return _scoreRule->getUpper();
            }
            return 42.4242;
        }

        void AnnoFilterManager::setCommonEnabled(bool enable) {
            if(enable != _commonEnabled) {
                _commonEnabled = enable;
                if(_curFilter != NULL) {
                    _curFilter->deactivate();
                }
                rebuildCurrentFilter();

                if(_curFilter != NULL) {
                    if(!isEnabled()) {
                        _curFilter->deactivate();
                    } else {
                        applyFilter();
                    }
                }
                emit filterEnable(_commonEnabled, _scoreEnabled);
            }
        }

        void AnnoFilterManager::setScoreEnabled(bool enable) {
            if(enable != _scoreEnabled) {
                _scoreEnabled = enable;
                if(_curFilter != NULL) {
                    _curFilter->deactivate();
                }
                rebuildCurrentFilter();

                if(_curFilter != NULL) {
                    if(!isEnabled()) {
                        _curFilter->deactivate();
                    } else {
                        applyFilter();
                    }
                }
                emit filterEnable(_commonEnabled, _scoreEnabled);
            }
        }


        void AnnoFilterManager::setEnabled(bool enable) {
            if(enable != _commonEnabled || enable != _scoreEnabled) {
                _commonEnabled = enable;
                _scoreEnabled = enable;
                if(_curFilter != NULL) {
                    _curFilter->deactivate();
                }
                rebuildCurrentFilter();

                if(_curFilter != NULL) {
                    if(!isEnabled()) {
                        _curFilter->deactivate();
                    } else {
                        applyFilter();
                    }
                }
                emit filterEnable(_commonEnabled, _scoreEnabled);
            }
        }

        void AnnoFilterManager::selectFilter(const QString &filterName) {
            GlobalLogger::instance()->logDebug(QString("Select filter [%1]").arg(filterName));

            if((_commonFilter != NULL && _commonFilter->getName() == filterName) ||
                    (_commonFilter == NULL && filterName.isEmpty()) ||
                    (!filterName.isEmpty() && !_filters->contains(filterName))) {
                return;
            }

            if(_commonEnabled) {
                if(_scoreEnabled) {
                    if(_commonFilter != NULL) {
                        _commonFilter->resetFilter();
                    }

                    if(filterName.isEmpty()) {
                        _commonFilter = _emptyFilter;
                    } else {
                        _commonFilter = _filters->value(filterName);
                    }

                    _commonFilter->setFileData(GlobalProjectManager::instance()->selectedFile());
                    rebuildCurrentFilter();
                    applyFilter();
                } else {
                    if(_curFilter != NULL) {
                        disconnectFilterSignals();
                        _curFilter->resetFilter();
                    }
                    if(filterName.isEmpty()) {
                        _commonFilter = _emptyFilter;
                    } else {
                        _commonFilter = _filters->value(filterName);
                    }

                    _curFilter = _commonFilter;
                    _curFilter->setFileData(GlobalProjectManager::instance()->selectedFile());
                    connectFilterSignals();
                    applyFilter();
                }
            } else {
                if(_commonFilter != NULL) {
                    _commonFilter->resetFilter();
                }
                if(filterName.isEmpty()) {
                    _commonFilter = _emptyFilter;
                } else {
                    _commonFilter = _filters->value(filterName);
                }
                _commonFilter->setFileData(GlobalProjectManager::instance()->selectedFile());
            }
            emit filterSelected(filterName);

//			if(filterName.isEmpty())
//			{
//				if(_curFilter != NULL)
//				{
//					disconnectFilterSignals();
//					_curFilter->resetFilter();
//				}
//				_curFilter = NULL;
//				emit filterSelected(filterName);
//			}
//			else if(_filters->contains(filterName))
//			{
//				if(_curFilter != NULL)
//				{
//					disconnectFilterSignals();
//					_curFilter->resetFilter();
//				}
//				_curFilter = _filters->value(filterName);
//				_curFilter->setFileData(GlobalProjectManager::instance()->selectedFile());
//				connectFilterSignals();
//				if(isEnabled())
//					applyFilter();
//				emit filterSelected(filterName);
//			}
        }

        void AnnoFilterManager::applyFilter() {
            if(_curFilter != NULL) {
                _curFilter->applyFilter();
            }
        }

        int AnnoFilterManager::filterCount() const {
            return  _filters->size();
        }

        AnnoFilter *AnnoFilterManager::getFilter(const QString &filterName) {
            return _filters->value(filterName, NULL);
        }

        QList<AnnoFilter *> AnnoFilterManager::getAllFilters() const {
            return _filters->values();
        }

        AnnoFilter *AnnoFilterManager::curFilter() {
            return _curFilter;
        }

        bool AnnoFilterManager::containsFilter(const QString &filterName) const {
            return _filters->contains(filterName);
        }

        bool AnnoFilterManager::containsFilter(AnnoFilter *filter) const {
            return _filters->contains(filter->getName());
        }

        bool AnnoFilterManager::addFilter(AnnoFilter *filter) {
            if(_filters->contains(filter->getName())) {
                return false;
            }

            _filters->insert(filter->getName(), filter);
            emit filterAdded(filter);
            return true;
        }

        bool AnnoFilterManager::isEnabled() const {
            return (_commonEnabled || _scoreEnabled);
        }

        bool AnnoFilterManager::removeFilter(const QString &filterName, bool delFilter) {
            AnnoFilter *filter = _filters->value(filterName, NULL);
            if(filter != NULL && _filters->remove(filterName) > 0) {
                if(_commonFilter == filter) {
                    setCommonEnabled(false);
                    selectFilter(QString());
                }

                if(delFilter) {
                    delete filter;
                }
                emit filterRemoved(filterName);
                return true;
            }
            return false;
        }

        int AnnoFilterManager::colorCount() const {
            return _colorRules->size();
        }

        graphics::VisualShapeConfig AnnoFilterManager::getColoringFor(dt::Annotation *anno) const {
            if(!_coloringEnabled) {
                return getDefaultColoring();
            }

            foreach(ColorFilterEntry * fe, *_colorRules) {
                if(fe->eval(anno)) {
                    return fe->getColoring();
                }
            }

            return getDefaultColoring();
        }

        graphics::VisualShapeConfig AnnoFilterManager::getDefaultColoring() {
            return GlobalConfig::instance()->getShapeConfig("default");
        }

        bool AnnoFilterManager::isColoringEnabled() const {
            return _coloringEnabled;
        }

        void AnnoFilterManager::setColoringEnable(bool state) {
            if(state != _coloringEnabled) {
                _coloringEnabled = state;
                emit coloringEnable(state);
                emit coloringUpdateNeeded();
            }
        }

        ColorFilterEntry *AnnoFilterManager::getColorRule(int index) {
            ColorFilterEntry *entry = NULL;
            if(index >= 0 && index < _colorRules->size()) {
                entry = _colorRules->at(index);
            }

            return entry;
        }

        QList<ColorFilterEntry *> AnnoFilterManager::getAllColorRules() const {
            return *_colorRules;
        }

        void AnnoFilterManager::addColorRule(ColorFilterEntry *rule) {
            if(rule != NULL) {
                _colorRules->append(rule);
                emit coloringAdded(rule, _colorRules->size() - 1);

                if(_coloringEnabled) {
                    emit coloringUpdateNeeded();
                }
            }
        }

        void AnnoFilterManager::insertColorRule(int index, ColorFilterEntry *rule) {
            if(rule != NULL && index >= 0 && index <= _colorRules->size()) {
                _colorRules->insert(index, rule);
                emit coloringAdded(rule, index);

                if(_coloringEnabled) {
                    emit coloringUpdateNeeded();
                }
            }
        }

        void AnnoFilterManager::removeColorRule(int index, bool delRule) {
            if(index >= 0 && index < _colorRules->size()) {
                QString name = _colorRules->at(index)->getFilter()->getName();
                if(delRule) {
                    delete _colorRules->at(index);
                }
                _colorRules->removeAt(index);
                emit coloringRemoved(name, index);

                if(_coloringEnabled) {
                    emit coloringUpdateNeeded();
                }
            }
        }

        void AnnoFilterManager::moveColorRule(int index, int newIndex) {
            //TODO beware of newIndex issue when newIndex > index
            if(index != newIndex && index >= 0 && index < _colorRules->size() && newIndex >= 0 && newIndex <= _colorRules->size()) {
                ColorFilterEntry *tmp = _colorRules->at(index);
                _colorRules->removeAt(index);
                _colorRules->insert(newIndex, tmp);
                emit coloringMoved(index, newIndex);

                if(_coloringEnabled) {
                    emit coloringUpdateNeeded();
                }
            }
        }

        void AnnoFilterManager::incColorRule(int index) {
            if(index > 0) {
                moveColorRule(index, index - 1);

                if(_coloringEnabled) {
                    emit coloringUpdateNeeded();
                }
            }
        }

        void AnnoFilterManager::decColorRule(int index) {
            if(index > _colorRules->size() - 1) {
                moveColorRule(index, index + 1);

                if(_coloringEnabled) {
                    emit coloringUpdateNeeded();
                }
            }
        }

        void AnnoFilterManager::updateColoring() {
            emit coloringUpdateNeeded();
        }

        void AnnoFilterManager::onPM_annoFileSelectChanged(int row, QUuid imageId, ::anno::dt::AnnoFileData *annoFile) {

            if(row >= 0 && annoFile != NULL) {
                assert(annoFile->imageInfo() != NULL);
                printf("Annotation: %d, image: %s\n", row, annoFile->imageInfo()->imagePath().absoluteFilePath().toStdString().c_str());

                if(_commonFilter != NULL) {
                    _commonFilter->setFileData(annoFile);
                }
                if(_scoreFilter != NULL) {
                    _scoreFilter->setFileData(annoFile);
                }
                if(_unifiedFilter != NULL) {
                    _unifiedFilter->setFileData(annoFile);
                }

                if(isEnabled() && _curFilter != NULL) {
                    applyFilter();
                }
            }
        }

    } //end: namespace filter
} //end: namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
