#include "include/NullPointerException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {

        NullPointerException::NullPointerException(const char *file, int line) :
            AnnoException(file, line) {
        }

        NullPointerException::NullPointerException(const char *file, int line, const QString &msg,
                AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        NullPointerException::NullPointerException(const char *file, int line, const char *msg,
                AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        NullPointerException::~NullPointerException() {
            if (_cause != 0) {
                delete _cause;
            }
        }

        const char *const NullPointerException::name() const {
            return "NullPointerException";
        }

    } //end namespace exc
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
