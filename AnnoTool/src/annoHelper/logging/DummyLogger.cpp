#include "include/DummyLogger.h"

namespace logging {

    /**
     * Constructor. A Logger can only be created if a name is given.
     * Per default, logging is disabled.
     *
     * @param name This logger's name.
     * @param curLevel This logger's minimum LogLevel.
     */
    DummyLogger::DummyLogger(const QString &name, LogLevel curLevel) :
        Logger(name, curLevel) {
    }

    DummyLogger::~DummyLogger() {
    }

    void DummyLogger::log(LogLevel level, const QString &timestamp,
                          const QString &msg) {
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
