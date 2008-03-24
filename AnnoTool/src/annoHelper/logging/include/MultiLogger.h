#ifndef MULTILOGGER_H_
#define MULTILOGGER_H_

#include "Logger.h"
#include <QList>

namespace logging {

    class MultiLogger : public Logger {
        private:
            QList<Logger *> _children;
            bool _autoDelete;

        public:
            MultiLogger(const QString &name, LogLevel curLevel = Debug);
            MultiLogger(const QString &name, bool autoDelete);
            MultiLogger(const QString &name, LogLevel curLevel, bool autoDelete);
            virtual ~MultiLogger();

        public:
            virtual void changeName(const QString &newName);
            virtual void setLoggingStatus(bool enabled);
            virtual void setLogLevel(LogLevel level);

        public:
            void addLogger(Logger *logger);

        public:
            virtual void log(LogLevel level, const QString &timestamp,
                             const QString &msg);
    };

}

#endif /*MULTILOGGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
