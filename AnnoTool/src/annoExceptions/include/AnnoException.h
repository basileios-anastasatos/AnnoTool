#ifndef ANNOEXCEPTION_H_
#define ANNOEXCEPTION_H_

#include <QString>

//namespace AnnoTool
namespace anno {
    //namespace Exceptions
    namespace exc {
        class AnnoException {
            protected:
                QString _file;
                int _line;
                QString _msg;
                AnnoException *_cause;

            public:
                AnnoException(const char *file, int line);
                AnnoException(const char *file, int line, const QString &msg,
                              AnnoException *cause = 0);
                AnnoException(const char *file, int line, const char *msg,
                              AnnoException *cause = 0);
                virtual ~AnnoException();

            public:
                virtual const char *const name() const;
                virtual const QString &msg() const;
                virtual const AnnoException *const cause() const;

                virtual void printTrace() const;
                virtual QString getTrace() const;
        };
    } //end namespace exc
} //end namespace anno

#endif /*ANNOEXCEPTION_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
