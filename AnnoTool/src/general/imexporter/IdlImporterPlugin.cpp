#include "include/IdlImporterPlugin.h"

namespace anno {

    IdlImporterPlugin::IdlImporterPlugin() {
    }

    IdlImporterPlugin::~IdlImporterPlugin() {
    }

    QString IdlImporterPlugin::name() const {
        return QString("Default IDL Importer");
    }

    QString IdlImporterPlugin::author() const {
        return QString("Oliver Schwahn");
    }

    QString IdlImporterPlugin::description() const {
        return QString();
    }

    bool IdlImporterPlugin::singleFileImport() const {
        return false;
    }

    bool IdlImporterPlugin::singleAnnotationImport() const {
        return false;
    }

    bool IdlImporterPlugin::exec(const QFileInfo &startDir) {
        return false;
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
