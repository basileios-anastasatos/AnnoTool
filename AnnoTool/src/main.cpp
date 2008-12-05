#include "AnnoToolMainWindow.h"
#include <QtGui>
#include <QApplication>
#include "importGlobals.h"
#include "IdlImporterPlugin.h"
#include "IdlExporterPlugin.h"

int main(int argc, char *argv[]) {
    GlobalLogger::instance()->logInfo(QString("%1 Version %2 is starting up, running on %3").arg(GlobalInfo::appName).arg(GlobalInfo::appVersionString()).arg(GlobalInfo::osName()));

    // Initialize Global Configuration
    // ------------------------------------------
    GlobalConfig::instance()->loadConfig();
    GlobalLogger::instance()->setLogLevel((logging::LogLevel)GlobalConfig::instance()->getInt("logging.loglevel"));
    // ------------------------------------------

    // Application Configuration
    // ------------------------------------------
    QApplication a(argc, argv);
    a.setApplicationName(GlobalInfo::appName);
    a.setApplicationVersion(GlobalInfo::appVersionString());
    a.setOrganizationName(GlobalInfo::orgName);
    a.setOrganizationDomain(GlobalInfo::orgDomain);
    // ------------------------------------------

    // Im- & Exporter Plugins
    //----------------------------------------------------------------
    GlobalImExportManager *ieman = GlobalImExportManager::instance();
    ieman->addImporter(new anno::IdlImporterPlugin());
    ieman->addExporter(new anno::IdlExporterPlugin());
    //----------------------------------------------------------------


    AnnoToolMainWindow w;
    w.show();
    a.connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(onAppClose()));
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    GlobalLogger::instance()->logDebug("entering QApplication loop");
    return a.exec();
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
