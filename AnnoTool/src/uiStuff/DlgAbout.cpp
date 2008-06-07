#include "include/DlgAbout.h"
#include "importGlobals.h"

DlgAbout::DlgAbout(QWidget *parent) :
    QDialog(parent) {
    ui.setupUi(this);
    setWindowFlags(Qt::Tool);

#ifdef QT_DEBUG
    setWindowTitle("About AnnoTool v2 [Debug Version]");
#endif
}

DlgAbout::~DlgAbout() {
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
