#include "include/IdlExporterPlugin.h"

namespace anno {

    IdlExporterPlugin::IdlExporterPlugin() {
    }

    IdlExporterPlugin::~IdlExporterPlugin() {
    }

    QString IdlExporterPlugin::name() const {
        return QString("Default IDL Exporter");
    }

    QString IdlExporterPlugin::author() const {
        return QString("Oliver Schwahn");
    }

    QString IdlExporterPlugin::description() const {
        return QString();
    }

    bool IdlExporterPlugin::exec(const QFileInfo &startDir) {
        return false;
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
