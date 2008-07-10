#ifndef FORMATEXCEPTION_H_
#define FORMATEXCEPTION_H_

#include "AnnoException.h"

namespace anno {
    namespace exc {

        class FormatException : public anno::exc::AnnoException {
            public:
                FormatException(const char *file, int line);
                FormatException(const char *file, int line, const QString &msg,
                                AnnoException *cause = 0);
                FormatException(const char *file, int line, const char *msg,
                                AnnoException *cause = 0);
                virtual ~FormatException();

            public:
                const char *const name() const;
        };

    }
}

#endif /*FORMATEXCEPTION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
