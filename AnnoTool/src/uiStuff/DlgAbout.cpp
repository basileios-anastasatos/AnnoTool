#include "include/DlgAbout.h"
#include "importGlobals.h"

DlgAbout::DlgAbout(QWidget *parent) :
    QDialog(parent) {
    setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
    ui.setupUi(this);

#ifdef QT_DEBUG
    setWindowTitle(QString("%1 [Debug-Version: %2 - %3] %4").arg(GlobalInfo::appName).arg(GlobalInfo::appVersionString()).arg(GlobalInfo::compileDateTime()).arg(GlobalInfo::isExperimental ? QString("- EXPERIMENTAL Build") : QString()));
#else
    setWindowTitle(QString("%1 [Release-Version: %2 - %3] %4").arg(GlobalInfo::appName).arg(GlobalInfo::appVersionString()).arg(GlobalInfo::compileDateTime()).arg(GlobalInfo::isExperimental ? QString("- EXPERIMENTAL Build") : QString()));
#endif

    ui.lbCaption->setText(ui.lbCaption->text().arg(GlobalInfo::appName).arg(GlobalInfo::appSubtitle).arg(GlobalInfo::appVersionString()).arg(QString("%1 Build: %2").arg(GlobalInfo::isRelease ? "Release" : "Debug").arg(GlobalInfo::compileDateTime())).arg(GlobalInfo::appBuildInfo));
    ui.lbAuthor->setText(ui.lbAuthor->text().arg(GlobalInfo::appAuthor).arg(GlobalInfo::orgName).arg(GlobalInfo::orgDep).arg(GlobalInfo::orgUni).arg(GlobalInfo::orgWebsite).arg(GlobalInfo::orgWebsite));
    ui.lbInfo->setText(ui.lbInfo->text().arg(GlobalInfo::compileQtVersion).arg(qVersion()).arg(GlobalInfo::osName()));
}

DlgAbout::~DlgAbout() {
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
