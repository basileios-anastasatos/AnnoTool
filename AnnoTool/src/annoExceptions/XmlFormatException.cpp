#include "include/XmlFormatException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {

        XmlFormatException::XmlFormatException(const char *file, int line) :
            XmlException(file, line) {
        }

        XmlFormatException::XmlFormatException(const char *file, int line,
                                               const QString &msg, AnnoException *cause) :
            XmlException(file, line, msg, cause) {
        }

        XmlFormatException::XmlFormatException(const char *file, int line,
                                               const char *msg, AnnoException *cause) :
            XmlException(file, line, msg, cause) {
        }

        XmlFormatException::~XmlFormatException() {
            if (_cause != 0) {
                delete _cause;
            }
        }

        const char *const XmlFormatException::name() const {
            return "XmlFormatException";
        }

    } //end namespace exc
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
