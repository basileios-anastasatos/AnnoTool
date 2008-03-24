#include "include/MultiLogger.h"

namespace logging {

    MultiLogger::MultiLogger(const QString &name, LogLevel curLevel) :
        Logger(name, curLevel) {
        _autoDelete = false;
    }

    MultiLogger::MultiLogger(const QString &name, bool autoDelete) :
        Logger(name, Debug) {
        _autoDelete = autoDelete;
    }

    MultiLogger::MultiLogger(const QString &name, LogLevel curLevel,
                             bool autoDelete) :
        Logger(name, curLevel) {
        _autoDelete = autoDelete;
    }

    MultiLogger::~MultiLogger() {
        if (_autoDelete && !_children.isEmpty()) {
            QListIterator<Logger *> i(_children);
            while (i.hasNext()) {
                delete i.next();
            }
        }
    }

    void MultiLogger::changeName(const QString &newName) {
        Logger::changeName(newName);
        if (!_children.isEmpty()) {
            QListIterator<Logger *> i(_children);
            while (i.hasNext()) {
                i.next()->changeName(newName);
            }
        }
    }

    void MultiLogger::setLoggingStatus(bool enabled) {
        Logger::setLoggingStatus(enabled);
        if (!_children.isEmpty()) {
            QListIterator<Logger *> i(_children);
            while (i.hasNext()) {
                i.next()->setLoggingStatus(enabled);
            }
        }
    }

    void MultiLogger::setLogLevel(LogLevel level) {
        Logger::setLogLevel(level);
        if (!_children.isEmpty()) {
            QListIterator<Logger *> i(_children);
            while (i.hasNext()) {
                i.next()->setLogLevel(level);
            }
        }
    }

    void MultiLogger::addLogger(Logger *logger) {
        logger->setLoggingStatus(_loggingEnabled);
        logger->setLogLevel(_curLogLevel);
        logger->changeName(_logName);
        _children.append(logger);
    }

    void MultiLogger::log(LogLevel level, const QString &timestamp,
                          const QString &msg) {
        if (_loggingEnabled && hasRightLevel(level) && !_children.isEmpty()) {
            QListIterator<Logger *> i(_children);
            while (i.hasNext()) {
                i.next()->log(level, timestamp, msg);
            }
        }
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
