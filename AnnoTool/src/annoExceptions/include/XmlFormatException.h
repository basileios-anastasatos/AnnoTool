#ifndef XMLFORMATEXCEPTION_H_
#define XMLFORMATEXCEPTION_H_

#include "XmlException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {
        class XmlFormatException : public XmlException {
            public:
                XmlFormatException(const char *file, int line);
                XmlFormatException(const char *file, int line, const QString &msg,
                                   AnnoException *cause = 0);
                XmlFormatException(const char *file, int line, const char *msg,
                                   AnnoException *cause = 0);
                virtual ~XmlFormatException();

            public:
                const char *const name() const;
        };
    } //end namespace exc
} //end namespace anno
#endif /*XMLFORMATEXCEPTION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
