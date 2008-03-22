#ifndef OUTOFRANGEEXCEPTION_H_
#define OUTOFRANGEEXCEPTION_H_

#include "AnnoException.h"

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {
        class OutOfRangeException : public AnnoException {
            public:
                OutOfRangeException(const char *file, int line);
                OutOfRangeException(const char *file, int line, const QString &msg,
                                    AnnoException *cause = 0);
                OutOfRangeException(const char *file, int line, const char *msg,
                                    AnnoException *cause = 0);
                virtual ~OutOfRangeException();

            public:
                const char *const name() const;
        };
    } //end namespace exc
} //end namespace anno

#endif /*OUTOFRANGEEXCEPTION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
