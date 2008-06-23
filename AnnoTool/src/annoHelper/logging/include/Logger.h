#ifndef LOGGER_H_
#define LOGGER_H_

#include <QString>

namespace logging {
    /**
     * LogLevel indicates the seriousness of a logged event.
     *
     * @author Oliver Schwahn
     */
    enum LogLevel {
        /**
         * Debug level.
         * Logged event is just useful for debugging and further developer info.
         */
        Debug = 0,

        /**
         * Info level.
         * Logged event is just for information purpose, nothing serious happened.
         */
        Info = 1,

        /**
         * Warning level.
         * Logged event gives a serious warning, something that should not be ignored happened.
         */
        Warning = 2,

        /**
         * Error level.
         * Logged event gives info about an error that occurred, something should be done. An
         * error could lead to a system/program crash (but hopefully it does not).
         */
        Error = 3,

        /**
         * Fatal error level.
         * Logged event gives info about a fatal error, something must be done. A fatal error
         * leads almost always to a system/program crash.
         */
        FatalError = 4
    };

    /**
     * Abstract Logging base class.
     * Provides methods useful for simple event-logging. Every logger must
     * have an unique name because it's used for identifying the logger.
     * Per default, a logger is disabled when it is created, so logging must be explicitly enabled.
     *
     * @author Oliver Schwahn
     */
    class Logger {
        protected:

            /**
             * This logger's name. This name should be added as source to every logged event.
             */
            QString _logName;

            /**
             * Indicates whether logging functionality is enabled or disabled.
             */
            bool _loggingEnabled;

            /**
             * All events of this LogLevel or above will be logged. All events below this LogLevel
             * won't be logged.
             */
            LogLevel _curLogLevel;

        protected:
            static QString getTimestamp();
            bool hasRightLevel(LogLevel level);

        public:
            Logger(const QString &name, LogLevel curLevel = Debug);
            virtual ~Logger();

            virtual void changeName(const QString &newName);
            void enableLogging();
            void disableLogging();
            virtual void setLoggingStatus(bool enabled);
            bool isEnabled();
            QString getLoggerName();
            virtual void setLogLevel(LogLevel level);
            void logDebug(const QString &msg);
            void logInfo(const QString &msg);
            void logWarning(const QString &msg);
            void logError(const QString &msg);
            void logFatalError(const QString &msg);
            void log(LogLevel level, const QString &msg);

            /**
             * Logs a given message with a given LogLevel and a given timestamp.
             *
             * @param level LogLevel to by applied.
             * @param timestamp Timestamp to be added.
             * @param msg Message to be logged.
             */
            virtual void log(LogLevel level, const QString &timestamp,
                             const QString &msg) = 0;

    };

}
//end namespace logging
#endif /*LOGGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
