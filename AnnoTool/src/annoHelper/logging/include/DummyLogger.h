#ifndef DUMMYLOGGER_H_
#define DUMMYLOGGER_H_

#include "Logger.h"

namespace logging {

    class DummyLogger : public Logger {
        public:
            DummyLogger(const QString &name, LogLevel curLevel = Debug);
            virtual ~DummyLogger();

        public:
            virtual void log(LogLevel level, const QString &timestamp,
                             const QString &msg);
    };

}

#endif /*DUMMYLOGGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
