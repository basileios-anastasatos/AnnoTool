#ifndef IOEXCEPTION_H_
#define IOEXCEPTION_H_

#include "AnnoException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {
        class IOException : public AnnoException {
            public:
                IOException(const char *file, int line);
                IOException(const char *file, int line, const QString &msg,
                            AnnoException *cause = 0);
                IOException(const char *file, int line, const char *msg,
                            AnnoException *cause = 0);
                virtual ~IOException();

            public:
                const char *const name() const;
        };
    } //end namespace exc
} //end namespace anno

#endif /*IOEXCEPTION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
