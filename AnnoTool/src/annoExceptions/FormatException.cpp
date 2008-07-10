#include "include/FormatException.h"

namespace anno {
    namespace exc {

        FormatException::FormatException(const char *file, int line) :
            AnnoException(file, line) {
        }

        FormatException::FormatException(const char *file, int line,
                                         const QString &msg, AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        FormatException::FormatException(const char *file, int line,
                                         const char *msg, AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        FormatException::~FormatException() {
            if(_cause != 0) {
                delete _cause;
            }
        }

        const char *const FormatException::name() const {
            return "FormatException";
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
