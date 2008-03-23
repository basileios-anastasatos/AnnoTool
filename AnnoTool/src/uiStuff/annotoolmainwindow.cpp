#include "include/annotoolmainwindow.h"
#include "include/zoomcontrol.h"

AnnoToolMainWindow::AnnoToolMainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);
    ZoomControl *z = new ZoomControl(ui.tbView);
    QAction *za = ui.tbView->addWidget(z);
    za->setVisible(true);
}

AnnoToolMainWindow::~AnnoToolMainWindow() {

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
