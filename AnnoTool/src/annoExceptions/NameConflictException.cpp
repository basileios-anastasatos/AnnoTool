#include "include/NameConflictException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {

        NameConflictException::NameConflictException(const char *file, int line) :
            AnnoException(file, line) {
        }

        NameConflictException::NameConflictException(const char *file, int line,
                const QString &msg, AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        NameConflictException::NameConflictException(const char *file, int line,
                const char *msg, AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        NameConflictException::~NameConflictException() {
        }

        const char *const NameConflictException::name() const {
            return "NameConflictException";
        }

    } //end namespace exc
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
