#ifndef ILLEGALSTATEEXCEPTION_H_
#define ILLEGALSTATEEXCEPTION_H_

#include "AnnoException.h"

namespace anno {
    namespace exc {

        class IllegalStateException : public anno::exc::AnnoException {
            public:
                IllegalStateException(const char *file, int line);
                IllegalStateException(const char *file, int line, const QString &msg,
                                      AnnoException *cause = 0);
                IllegalStateException(const char *file, int line, const char *msg,
                                      AnnoException *cause = 0);
                virtual ~IllegalStateException();

            public:
                const char *const name() const;
        };

    }
}

#endif /*ILLEGALSTATEEXCEPTION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
