#include "include/NoSuchElementException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {

        NoSuchElementException::NoSuchElementException(const char *file, int line) :
            AnnoException(file, line) {
        }

        NoSuchElementException::NoSuchElementException(const char *file, int line,
                const QString &msg, AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        NoSuchElementException::NoSuchElementException(const char *file, int line,
                const char *msg, AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        NoSuchElementException::~NoSuchElementException() {
            if (_cause != 0) {
                delete _cause;
            }
        }

        const char *const NoSuchElementException::name() const {
            return "NoSuchElementException";
        }

    } //end namespace exc
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
