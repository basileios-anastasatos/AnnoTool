#include "annotoolmainwindow.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    AnnoToolMainWindow w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
