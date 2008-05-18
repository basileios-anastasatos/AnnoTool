#include "annotoolmainwindow.h"

#include <QtGui>
#include <QApplication>
#include "importGlobals.h"
#include "IdlImporterPlugin.h"
#include "IdlExporterPlugin.h"


int main(int argc, char *argv[]) {
    GlobalLogger::instance()->logInfo("AnnoTool starting up");
    QApplication a(argc, argv);

    // Im- & Exporter Plugins
    //----------------------------------------------------------------
    GlobalImExportManager *ieman = GlobalImExportManager::instance();
    ieman->addImporter(new anno::IdlImporterPlugin());
    ieman->addExporter(new anno::IdlExporterPlugin());
    //----------------------------------------------------------------

    AnnoToolMainWindow w;
    w.show();
    a.connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(on_appClose()));
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    GlobalLogger::instance()->logDebug("entering QApplication loop");
    return a.exec();
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
