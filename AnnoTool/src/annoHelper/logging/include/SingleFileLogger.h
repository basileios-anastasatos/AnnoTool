#ifndef SINGLEFILELOGGER_H_
#define SINGLEFILELOGGER_H_

#include "Logger.h"

class QFile;
class QTextStream;

namespace logging {

    class SingleFileLogger : public Logger {
        private:
            QFile *_file;
            QTextStream *_out;

        private:
            void setupLogFile(const QString &name, bool append);

        public:
            SingleFileLogger(const QString &name, LogLevel curLevel = Debug);
            SingleFileLogger(const QString &name, bool append);
            SingleFileLogger(const QString &name, LogLevel curLevel, bool append);
            virtual ~SingleFileLogger();

        public:
            virtual void log(LogLevel level, const QString &timestamp,
                             const QString &msg);
    };

}

#endif /*SINGLEFILELOGGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
