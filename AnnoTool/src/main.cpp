#include "AnnoToolMainWindow.h"
#include <QtGui>
#include <QString>
#include <QStringList>
#include <string>
#include <iostream>
#include "importGlobals.h"
#include "IdlImporterPlugin.h"
#include "IdlExporterPlugin.h"
#include "AnnoQApplication.h"

int main(int argc, char *argv[]) {
    // Parse command line options
    const std::string help_message(
        "Usage:\n"
        "  -h|--help             : print this help message and exit\n"
        "  -o|--open <filename>  : open <filename>\n"
        "  -G|--noglobalfilters  : do not load global filters\n"
    );
    QString project_file;
    bool    globalFilters = true;
    for (int ii = 1; ii < argc; ++ii) {
        std::string ss(argv[ii]);
        if ((ss == "-h") ||
            (ss == "--help")) {
            std::cerr << help_message;
            return 0;
        } else if ((ss == "-o") ||
                   (ss == "--open")) {
            if (ii + 1 < argc) {
                project_file = argv[++ii];

            } else {
                std::cerr << "Error: missing argument to " << argv[ii] << std::endl;
                std::cerr << help_message;
                return -1;
            }
        } else if ((ss == "-G") ||
                   (ss == "--noglobalfilters")) {
            globalFilters = false;
        } else {
            std::cerr << "Error: Unrecognized argument " << argv[ii] << std::endl;
            std::cerr << help_message;
            return -1;
        }
    }

    GlobalLogger::instance()->logInfo(QString("%1 Version %2 is starting up, running on %3").arg(GlobalInfo::appName).arg(GlobalInfo::appVersionString()).arg(GlobalInfo::osName()));

    // Initialize Global Configuration
    // ------------------------------------------
    GlobalConfig::instance()->loadConfig();
    GlobalLogger::instance()->setLogLevel((logging::LogLevel)GlobalConfig::instance()->getInt("logging.loglevel"));
    // ------------------------------------------

    // Application Configuration
    // ------------------------------------------
    anno::AnnoQApplication a(argc, argv);
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

    // Process command line options
    if (!project_file.isNull()) {
         w.openAnnoProject(project_file, globalFilters);
    }
    w.show();
    a.connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(onAppClose()));
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    GlobalLogger::instance()->logDebug("entering QApplication loop");
    return a.exec();
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
