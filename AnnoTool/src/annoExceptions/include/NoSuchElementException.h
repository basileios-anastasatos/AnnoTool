#ifndef NOSUCHELEMENTEXCEPTION_H_
#define NOSUCHELEMENTEXCEPTION_H_

#include "AnnoException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {
        class NoSuchElementException : public AnnoException {
            public:
                NoSuchElementException(const char *file, int line);
                NoSuchElementException(const char *file, int line, const QString &msg,
                                       AnnoException *cause = 0);
                NoSuchElementException(const char *file, int line, const char *msg,
                                       AnnoException *cause = 0);
                virtual ~NoSuchElementException();

            public:
                const char *const name() const;
        };
    } //end namespace exc
} //end namespace anno

#endif /*NOSUCHELEMENTEXCEPTION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
