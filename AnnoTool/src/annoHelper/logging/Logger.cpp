#include "include/Logger.h"
#include <QDateTime>

namespace logging {
    /**
     * Constructor. A Logger can only be created if a name is given.
     * Per default, logging is disabled.
     *
     * @param name This logger's name.
     * @param curLevel This logger's minimum LogLevel.
     */
    Logger::Logger(const QString &name, LogLevel curLevel) {
        _logName = name;
        _curLogLevel = curLevel;
        _loggingEnabled = false;
    }

    /**
     * Standard destructor.
     */
    Logger::~Logger() {
    }

    /**
     * Gets the current timestamp. Current system time is formatted as "DD.MM.YYYY
     * hh:mm:ss".
     *
     * @return Current timestamp as string.
     */
    QString Logger::getTimestamp() {
        QDateTime time = QDateTime::currentDateTime();
        return time.toString("dd.MM.yyyy hh:mm:ss");
    }

    /**
     * Checks whether an event with a given LogLevel must be logged.
     *
     * @param level LogLevel to check.
     * @return True if event must be logged, false otherwise.
     */
    bool Logger::hasRightLevel(LogLevel level) {
        return (_curLogLevel <= level);
    }

    /**
     * Internal helper for changing the logger's name. Optimally this is never used, because
     * of problems that might appear by changing the name. So use with care!
     *
     * @param newName Logger's new name.
     */
    void Logger::changeName(const QString &newName) {
        _logName = newName;
    }

    /**
     * Enables logging functionality. If logging is disabled, all logged events disappear in
     * the great wide nirvana.
     *
     * @see Logger#disableLogging
     * @see Logger#setLoggingStatus
     */
    void Logger::enableLogging() {
        setLoggingStatus(true);
    }

    /**
     * Disables logging functionality. If logging is disabled, all logged events disappear
     * in the great wide nirvana.
     *
     * @see Logger#enableLogging
     * @see Logger#setLoggingStatus
     */
    void Logger::disableLogging() {
        setLoggingStatus(false);
    }

    /**
     * Enables or disables logging functionality. If logging is disabled, all logged events
     * disappear in the great wide nirvana.
     *
     * @param enabled If true, logging gets enabled, otherwise logging gets disabled.
     * @see Logger#enableLogging
     * @see Logger#disableLogging
     */
    void Logger::setLoggingStatus(bool enabled) {
        _loggingEnabled = enabled;
    }

    /**
     * Checks whether logging functionality is enabled.
     *
     * @return True, if logging is enabled, false otherwise.
     */
    bool Logger::isEnabled() {
        return _loggingEnabled;
    }

    /**
     * Gets this Logger's name.
     *
     * @return Logger's name.
     */
    QString Logger::getLoggerName() {
        return _logName;
    }

    /**
     * Changes the current LogLevel to the given one.
     *
     * @param level Logger's new LogLevel.
     */
    void Logger::setLogLevel(LogLevel level) {
        _curLogLevel = level;
    }

    /**
     * Logs a given message with LogLevel Debug. A timestamp is added automatically.
     *
     * @param msg Message to be logged.
     * @see Logger#log(LogLevel, String)
     */
    void Logger::logDebug(const QString &msg) {
        log(Debug, msg);
    }

    /**
     * Logs a given message with LogLevel Info. A timestamp is added automatically.
     *
     * @param msg Message to be logged.
     * @see Logger#log(LogLevel, String)
     */
    void Logger::logInfo(const QString &msg) {
        log(Info, msg);
    }

    /**
     * Logs a given message with LogLevel Warning. A timestamp is added automatically.
     *
     * @param msg Message to be logged.
     * @see Logger#log(LogLevel, String)
     */
    void Logger::logWarning(const QString &msg) {
        log(Warning, msg);
    }

    /**
     * Logs a given message with LogLevel Error. A timestamp is added automatically.
     *
     * @param msg Message to be logged.
     * @see Logger#log(LogLevel, String)
     */
    void Logger::logError(const QString &msg) {
        log(Error, msg);
    }

    /**
     * Logs a given message with LogLevel FatalError. A timestamp is added automatically.
     *
     * @param msg Message to be logged.
     * @see Logger#log(LogLevel, String)
     */
    void Logger::logFatalError(const QString &msg) {
        log(FatalError, msg);
    }

    /**
     * Logs a given message with a given LogLevel. A timestamp is added automatically.
     *
     * @param level LogLevel to by applied.
     * @param msg Message to be logged.
     */
    void Logger::log(LogLevel level, const QString &msg) {
        log(level, getTimestamp(), msg);
    }

} //end namespace logging

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
