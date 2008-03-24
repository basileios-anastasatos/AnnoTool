#ifndef CONSOLELOGGER_H_
#define CONSOLELOGGER_H_

#include "Logger.h"
#include <QTextStream>

namespace logging {

    class ConsoleLogger : public Logger {
        private:
            QTextStream _out;

        public:
            ConsoleLogger(const QString &name, LogLevel curLevel = Debug);
            virtual ~ConsoleLogger();

        public:
            virtual void log(LogLevel level, const QString &timestamp,
                             const QString &msg);
    };

}

#endif /*CONSOLELOGGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
