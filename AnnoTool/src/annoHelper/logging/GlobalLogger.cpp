#include "include/GlobalLogger.h"
#include "include/MultiLogger.h"
#include "include/ConsoleLogger.h"
#include "include/SingleFileLogger.h"

namespace logging {

    Logger *GlobalLogger::_logger = NULL;

    GlobalLogger::GlobalLogger() {
    }

    void GlobalLogger::setupLogger() {
        MultiLogger *multi = new MultiLogger("AnnoTool", Debug, true);
        ConsoleLogger *con = new ConsoleLogger("AnnoTool", Debug);
        SingleFileLogger *fil = new SingleFileLogger("AnnoTool", Debug, true);
        multi->addLogger(con);
        multi->addLogger(fil);
        multi->enableLogging();
        _logger = multi;
    }

    Logger *GlobalLogger::instance() {
        if(_logger == NULL) {
            setupLogger();
        }
        return _logger;
    }

}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
