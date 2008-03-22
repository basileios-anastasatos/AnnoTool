#include "include/AnnoException.h"
#include <iostream>

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {
        AnnoException::AnnoException(const char *file, int line) :
            _file(file), _line(line), _msg(""), _cause(0) {

        }
        AnnoException::AnnoException(const char *file, int line,
                                     const QString &msg, AnnoException *cause /*= NULL*/) :
            _file(file), _line(line), _msg(msg), _cause(cause) {
        }

        AnnoException::AnnoException(const char *file, int line, const char *msg,
                                     AnnoException *cause /*= NULL*/) :
            _file(file), _line(line), _msg(msg), _cause(cause) {
        }

        AnnoException::~AnnoException() {
            if (_cause != 0) {
                delete _cause;
            }
        }

        const char *const AnnoException::name() const {
            return "AnnoException";
        }

        const QString &AnnoException::msg() const {
            return _msg;
        }

        const AnnoException *const AnnoException::cause() const {
            return _cause;
        }

        void AnnoException::printTrace() const {
            std::cerr << name() << " occured in [" << _file.toUtf8().constData() << "] at line " << _line << "." << std::endl << "Message: "
                      << _msg.toUtf8().constData() << std::endl;
            if (_cause != 0) {
                std::cerr << "caused by..." << std::endl;
                _cause->printTrace();
            }
        }
        QString AnnoException::getTrace() const {
            QString tmp;
            tmp.append(name());
            tmp.append(" occured in [");
            tmp.append(_file);
            tmp.append("] at line ");
            tmp.append(QString::number(_line, 10));
            tmp.append(".");
            tmp.append("\n");
            tmp.append("Message: ");
            tmp.append(_msg);
            tmp.append("\n");
            if (_cause != 0) {
                tmp.append("caused by...");
                tmp.append("\n");
                tmp.append(_cause->getTrace());
            }

            return tmp;
        }

    } //end namespace exc
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
