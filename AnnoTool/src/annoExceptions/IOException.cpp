#include "include/IOException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {

        IOException::IOException(const char *file, int line) :
            AnnoException(file, line) {
        }

        IOException::IOException(const char *file, int line, const QString &msg,
                                 AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        IOException::IOException(const char *file, int line, const char *msg,
                                 AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        IOException::~IOException() {
            if (_cause != 0) {
                delete _cause;
            }
        }

        const char *const IOException::name() const {
            return "IOException";
        }

    } //end namespace exc
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
