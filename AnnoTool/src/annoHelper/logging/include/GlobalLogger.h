#ifndef GLOBALLOGGER_H_
#define GLOBALLOGGER_H_

#include "Logger.h"

namespace logging {

    class GlobalLogger {
        private:
            static Logger *_logger;

        private:
            GlobalLogger();
            static void setupLogger();

        public:
            static Logger *instance();

    };

}

#ifdef QT_DEBUG
    #define LOGDEBUG(msg) logging::GlobalLogger::instance()->logDebug(msg)
#else
    #define LOGDEBUG(msg)
#endif

#define LOGINFO(msg) logging::GlobalLogger::instance()->logInfo(msg)
#define LOGWARN(msg) logging::GlobalLogger::instance()->logWarn(msg)
#define LOGERR(msg) logging::GlobalLogger::instance()->logError(msg)
#define LOGFATAL(msg) logging::GlobalLogger::instance()->logFatalError(msg)

#endif /*GLOBALLOGGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
