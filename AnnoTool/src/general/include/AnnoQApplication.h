#ifndef ANNOQAPPLICATION_H_
#define ANNOQAPPLICATION_H_ ANNOQAPPLICATION_H_

/**
* @file AnnoQApplication.h
*
* Class AnnoQApplication differs from QApplication only in catching exceptions
* thrown by event handlers.
* 
* See: http://stackoverflow.com/questions/10665655/how-to-log-uncatched-exceptions-of-a-qapplication
*/

// Qt includes
// ----------------------------------
#include <QApplication>
#include <QObject>
#include <QEvent>
// ----------------------------------
// various default includes
// ----------------------------------
// - nothing
// ----------------------------------
// project includes
// ----------------------------------
#include "GlobalLogger.h"

namespace anno {
    class AnnoQApplication: public QApplication {
        public:
            AnnoQApplication(int argc, char *argv[]) :
                QApplication(argc, argv) {
            }

            virtual bool notify(QObject *receiver, QEvent *event) {
                try {
                    return QApplication::notify(receiver, event);
                } catch (anno::exc::AnnoException *e) {
                    LOGERR(e->msg());
                    delete e;
                } catch (...) {
                    LOGERR("Unknown exception thrown!");
                }
            }
    };
}

#endif

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
