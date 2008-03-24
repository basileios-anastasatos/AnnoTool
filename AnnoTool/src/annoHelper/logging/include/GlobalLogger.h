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
#endif /*GLOBALLOGGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
