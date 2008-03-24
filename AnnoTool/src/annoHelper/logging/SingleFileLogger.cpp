#include "include/SingleFileLogger.h"
#include <QFile>
#include <QTextStream>

namespace logging {

    SingleFileLogger::SingleFileLogger(const QString &name, LogLevel curLevel) :
        Logger(name, curLevel) {
        setupLogFile(name, true);
    }

    SingleFileLogger::SingleFileLogger(const QString &name, bool append) :
        Logger(name, Debug) {
        setupLogFile(name, append);
    }

    SingleFileLogger::SingleFileLogger(const QString &name, LogLevel curLevel,
                                       bool append) :
        Logger(name, curLevel) {
        setupLogFile(name, append);
    }

    SingleFileLogger::~SingleFileLogger() {
        if (_out != NULL) {
            delete _out;
        }
        delete _file;
    }

    void SingleFileLogger::setupLogFile(const QString &name, bool append) {
        _out = NULL;
        _file = new QFile(name + ".log");
        QFile::OpenMode om = QFile::WriteOnly | QFile::Text;
        om |= append ? QFile::Append : QFile::Truncate;
        if (_file->open(om)) {
            _out = new QTextStream(_file);
        }
    }

    void SingleFileLogger::log(LogLevel level, const QString &timestamp,
                               const QString &msg) {
        if (_out != NULL && _loggingEnabled && hasRightLevel(level)) {
            (*_out) << "[" << timestamp << "][" << _logName << "][" << level << "] -> "
                    << msg << endl << flush;
        }
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
