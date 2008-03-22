#ifndef ANNOTOOLMAINWINDOW_H
#define ANNOTOOLMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_annotoolmainwindow.h"

class AnnoToolMainWindow : public QMainWindow {
        Q_OBJECT

    public:
        AnnoToolMainWindow(QWidget *parent = 0);
        ~AnnoToolMainWindow();

    private:
        Ui::AnnoToolMainWindowClass ui;
};

#endif // ANNOTOOLMAINWINDOW_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
