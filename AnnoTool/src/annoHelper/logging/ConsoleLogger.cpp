#include "include/ConsoleLogger.h"

namespace logging {

    /**
     * Constructor. A Logger can only be created if a name is given.
     * Per default, logging is disabled.
     *
     * @param name This logger's name.
     * @param curLevel This logger's minimum LogLevel.
     */
    ConsoleLogger::ConsoleLogger(const QString &name, LogLevel curLevel) :
        Logger(name, curLevel), _out(stdout) {
    }

    ConsoleLogger::~ConsoleLogger() {
    }

    void ConsoleLogger::log(LogLevel level, const QString &timestamp,
                            const QString &msg) {
        if (_loggingEnabled && hasRightLevel(level)) {
            _out << "[" << timestamp << "][" << _logName << "][" << level << "] -> " << msg
                 << endl << flush;
        }
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
