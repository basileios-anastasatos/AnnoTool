#include "include/XmlException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {

        XmlException::XmlException(const char *file, int line) :
            AnnoException(file, line) {
        }

        XmlException::XmlException(const char *file, int line, const QString &msg,
                                   AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        XmlException::XmlException(const char *file, int line, const char *msg,
                                   AnnoException *cause) :
            AnnoException(file, line, msg, cause) {
        }

        XmlException::~XmlException() {
            if (_cause != 0) {
                delete _cause;
            }
        }

        const char *const XmlException::name() const {
            return "XmlException";
        }

    } //end namespace exc
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
