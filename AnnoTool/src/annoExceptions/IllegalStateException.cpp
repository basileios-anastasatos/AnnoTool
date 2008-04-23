#include "include/IllegalStateException.h"

namespace anno {

    namespace exc {

        IllegalStateException::IllegalStateException(const char *file, int line) :
            AnnoException(file, line) {
        }

        IllegalStateException::IllegalStateException(const char *file, int line,
                const QString &msg, AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        IllegalStateException::IllegalStateException(const char *file, int line,
                const char *msg, AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        IllegalStateException::~IllegalStateException() {
        }

        const char *const IllegalStateException::name() const {
            return "IllegalStateException";
        }

    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
