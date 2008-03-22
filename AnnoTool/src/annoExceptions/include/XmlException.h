#ifndef XMLEXCEPTION_H_
#define XMLEXCEPTION_H_

#include "AnnoException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {
        class XmlException : public AnnoException {
            public:
                XmlException(const char *file, int line);
                XmlException(const char *file, int line, const QString &msg,
                             AnnoException *cause = 0);
                XmlException(const char *file, int line, const char *msg,
                             AnnoException *cause = 0);
                virtual ~XmlException();

            public:
                const char *const name() const;
        };
    } //end namespace exc
} //end namespace anno

#endif /*XMLEXCEPTION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
