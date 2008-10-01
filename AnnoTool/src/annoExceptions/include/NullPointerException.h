#ifndef NULLPOINTEREXCEPTION_H_
#define NULLPOINTEREXCEPTION_H_

#include "AnnoException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {
        class NullPointerException : public AnnoException {
            public:
                NullPointerException(const char *file, int line);
                NullPointerException(const char *file, int line, const QString &msg,
                                     AnnoException *cause = 0);
                NullPointerException(const char *file, int line, const char *msg,
                                     AnnoException *cause = 0);
                virtual ~NullPointerException();

            public:
                const char *const name() const;
        };
    } //end namespace exc
} //end namespace anno

#endif /* NULLPOINTEREXCEPTION_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
