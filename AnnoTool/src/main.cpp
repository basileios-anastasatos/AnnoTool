#include "AnnoToolMainWindow.h"
#include <QtGui>
#include <QApplication>
#include <QString>
#include <string>
#include <iostream>
#include "importGlobals.h"
#include "IdlImporterPlugin.h"
#include "IdlExporterPlugin.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    GlobalLogger::instance()->logInfo(QString("%1 Version %2 is starting up, running on %3").arg(GlobalInfo::appName).arg(GlobalInfo::appVersionString()).arg(GlobalInfo::osName()));

    // Declare command line options
    po::options_description desc("Command line options");
    desc.add_options()
        ("help,h",                           "produce help message")
        ("open,o", po::value<std::string>(), "open a project")
        // more command line options here
    ;

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

    // Parse command line options
    po::variables_map vm;
    try {
        // http://www.boost.org/doc/libs/1_54_0/doc/html/boost/program_options/command_line_style/style_t.html
        po::command_line_style::style_t style = po::command_line_style::style_t(
            po::command_line_style::allow_short          |
            po::command_line_style::allow_dash_for_short |
            po::command_line_style::short_allow_adjacent |
            po::command_line_style::short_allow_next     |
            po::command_line_style::allow_long           |
            po::command_line_style::long_allow_adjacent  |
            po::command_line_style::long_allow_next      |
            po::command_line_style::allow_sticky         |
         // po::command_line_style::allow_guessing       |
            po::command_line_style::allow_dash_for_short |
            (0 & ~0));
        po::store(po::parse_command_line(a.argc(), a.argv(), desc, style), vm);
        po::notify(vm);
    } catch (po::error &ee) {
        std::cerr << "Error while parsing the command line." << "\n";
        std::cerr << ee.what()                               << "\n";
        std::cerr << desc                                    << "\n";
        return 1;
    }

    // Im- & Exporter Plugins
    //----------------------------------------------------------------
    GlobalImExportManager *ieman = GlobalImExportManager::instance();
    ieman->addImporter(new anno::IdlImporterPlugin());
    ieman->addExporter(new anno::IdlExporterPlugin());
    //----------------------------------------------------------------

    AnnoToolMainWindow w;

    // Process command line options
    if (vm.count("help")) {
        std::cerr << desc << "\n";
        return 0;
    } else if (vm.count("open")) {
        w.openAnnoProject(QString(vm["open"].as<std::string>().c_str()));
    }

    w.show();
    a.connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(onAppClose()));
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    GlobalLogger::instance()->logDebug("entering QApplication loop");
    return a.exec();
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
