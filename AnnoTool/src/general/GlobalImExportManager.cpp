#include "include/GlobalImExportManager.h"
#include "importGlobals.h"

#include <QMutexLocker>

namespace anno {
    GlobalImExportManager *GlobalImExportManager::_me = NULL;

    GlobalImExportManager::GlobalImExportManager() {
    }

    GlobalImExportManager::~GlobalImExportManager() {
        deleteAllImporter();
        deleteAllExporter();
    }

    void GlobalImExportManager::setupGlobalImExportManager() {
        _me = new GlobalImExportManager();
    }

    GlobalImExportManager *GlobalImExportManager::instance() {
        if (_me == NULL) {
            setupGlobalImExportManager();
        }
        return _me;
    }

    void GlobalImExportManager::reset() {
        if (_me != NULL) {
            delete _me;
            _me = NULL;
        }
    }

    int GlobalImExportManager::importerCount() const {
        QMutexLocker locker(&_mutex);
        return _importer.size();
    }

    int GlobalImExportManager::exporterCount() const {
        QMutexLocker locker(&_mutex);
        return _exporter.size();
    }

    void GlobalImExportManager::addImporter(ImporterPlugin *importer) {
        QMutexLocker locker(&_mutex);
        if (importer != NULL) {
            _importer.append(importer);
        }
    }

    void GlobalImExportManager::addExporter(ExporterPlugin *exporter) {
        QMutexLocker locker(&_mutex);
        if (exporter != NULL) {
            _exporter.append(exporter);
        }
    }

    void GlobalImExportManager::insertImporter(int index, ImporterPlugin *importer) {
        QMutexLocker locker(&_mutex);
        if (importer != NULL) {
            if (index < 0) {
                index = 0;
            } else if (index > _importer.size()) {
                index = _importer.size();
            }
            _importer.insert(index, importer);
        }
    }

    void GlobalImExportManager::insertExporter(int index, ExporterPlugin *exporter) {
        QMutexLocker locker(&_mutex);
        if (exporter != NULL) {
            if (index < 0) {
                index = 0;
            } else if (index > _exporter.size()) {
                index = _exporter.size();
            }
            _exporter.insert(index, exporter);
        }
    }

    ImporterPlugin *GlobalImExportManager::getImporter(int index) const {
        QMutexLocker locker(&_mutex);
        if (index < 0 && index >= _importer.size()) {
            return NULL;
        }
        return _importer.at(index);
    }

    ExporterPlugin *GlobalImExportManager::getExporter(int index) const {
        QMutexLocker locker(&_mutex);
        if (index < 0 && index >= _exporter.size()) {
            return NULL;
        }
        return _exporter.at(index);
    }

    void GlobalImExportManager::removeImporter(int index) {
        QMutexLocker locker(&_mutex);
        if (index >= 0 && index < _importer.size()) {
            _importer.removeAt(index);
        }
    }

    void GlobalImExportManager::removeExporter(int index) {
        QMutexLocker locker(&_mutex);
        if (index >= 0 && index < _exporter.size()) {
            _exporter.removeAt(index);
        }
    }

    void GlobalImExportManager::removeAllImporter() {
        QMutexLocker locker(&_mutex);
        _importer.clear();
    }

    void GlobalImExportManager::removeAllExporter() {
        QMutexLocker locker(&_mutex);
        _exporter.clear();
    }

    void GlobalImExportManager::deleteImporter(int index) {
        QMutexLocker locker(&_mutex);
        if (index >= 0 && index < _importer.size()) {
            delete _importer.at(index);
            _importer.removeAt(index);
        }
    }

    void GlobalImExportManager::deleteExporter(int index) {
        QMutexLocker locker(&_mutex);
        if (index >= 0 && index < _exporter.size()) {
            delete _exporter.at(index);
            _exporter.removeAt(index);
        }
    }

    void GlobalImExportManager::deleteAllImporter() {
        QMutexLocker locker(&_mutex);
        if (!_importer.isEmpty()) {
            QListIterator<ImporterPlugin *> i(_importer);
            while (i.hasNext()) {
                delete i.next();
            }
            _importer.clear();
        }
    }

    void GlobalImExportManager::deleteAllExporter() {
        QMutexLocker locker(&_mutex);
        if (!_exporter.isEmpty()) {
            QListIterator<ExporterPlugin *> i(_exporter);
            while (i.hasNext()) {
                delete i.next();
            }
            _exporter.clear();
        }
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
