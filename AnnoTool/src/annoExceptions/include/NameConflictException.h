#ifndef NAMECONFLICTEXCEPTION_H_
#define NAMECONFLICTEXCEPTION_H_

#include "AnnoException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {
        class NameConflictException : public AnnoException {
            public:
                NameConflictException(const char *file, int line);
                NameConflictException(const char *file, int line, const QString &msg,
                                      AnnoException *cause = 0);
                NameConflictException(const char *file, int line, const char *msg,
                                      AnnoException *cause = 0);
                virtual ~NameConflictException();

            public:
                const char *const name() const;
        };
    } //end namespace exc
} //end namespace anno
#endif /*NAMECONFLICTEXCEPTION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
