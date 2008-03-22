#include "include/OutOfRangeException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {

        OutOfRangeException::OutOfRangeException(const char *file, int line) :
            AnnoException(file, line) {

        }
        OutOfRangeException::OutOfRangeException(const char *file, int line,
                const QString &msg, AnnoException *cause) :
            AnnoException(file, line, msg, cause) {

        }
        OutOfRangeException::OutOfRangeException(const char *file, int line,
                const char *msg, AnnoException *cause) :
            AnnoException(file, line, msg, cause) {

        }

        OutOfRangeException::~OutOfRangeException() {
        }

        const char *const OutOfRangeException::name() const {
            return "OutOfRangeException";
        }

    } //end namespace exc
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
