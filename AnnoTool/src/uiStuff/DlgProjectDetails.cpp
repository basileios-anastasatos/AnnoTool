#include "include/DlgProjectDetails.h"
#include "include/DlgFileChooser.h"
#include "GlobalLogger.h"
#include "GlobalProjectManager.h"
#include "GlobalConfig.h"
#include <QMessageBox>

using ::logging::GlobalLogger;
using ::anno::GlobalProjectManager;
using ::anno::GlobalConfig;

DlgProjectDetails::DlgProjectDetails(QWidget *parent) :
    QDialog(parent) {
    ui.setupUi(this);
    _modified = false;
    setupActions();
    initData();
}

DlgProjectDetails::~DlgProjectDetails() {

}

void DlgProjectDetails::setupActions() {
    ui.btClassPathAdd->setDefaultAction(ui.actionClassPathAdd);
    ui.btClassPathRem->setDefaultAction(ui.actionClassPathRem);

    ui.btSearchPathAdd->setDefaultAction(ui.actionSearchPathAdd);
    ui.btSearchPathRem->setDefaultAction(ui.actionSearchPathRem);
}

void DlgProjectDetails::initData() {
    if (GlobalProjectManager::instance()->isValid()) {
        GlobalProjectManager *pm = GlobalProjectManager::instance();
        ui.editProjectName->setText(pm->project()->projectName());
        ui.editProjectUuid->setText(pm->project()->uuidAsString());

        if (!pm->project()->classPath()->isEmpty()) {
            QListIterator<QFileInfo> i(*pm->project()->classPath());
            while (i.hasNext()) {
                QString txt = i.next().filePath();
                QListWidgetItem *item = new QListWidgetItem(ui.lstClasspath);
                item->setText(txt);
                item->setToolTip(txt);
                ui.lstClasspath->addItem(item);
            }
        }
        if (!pm->project()->searchPath()->isEmpty()) {
            QListIterator<QFileInfo> i(*pm->project()->searchPath());
            while (i.hasNext()) {
                QString txt = i.next().filePath();
                QListWidgetItem *item = new QListWidgetItem(ui.lstSearchpath);
                item->setText(txt);
                item->setToolTip(txt);
                ui.lstSearchpath->addItem(item);
            }
        }
    }
}

void DlgProjectDetails::on_actionClassPathAdd_triggered() {
    GlobalLogger::instance()->logDebug("Classpath add...");
    DlgFileChooser *dlg = new DlgFileChooser(this, false);
    dlg->setSelectFilter(QString("AnnoClass File (*.%1)").arg(GlobalConfig::fileExt.classDefs));
    if (dlg->exec() == QDialog::Accepted) {
        QFileInfo file(dlg->selectedPath());
        if (file.isFile()) {
            QString txt = file.filePath();
            QListWidgetItem *item = new QListWidgetItem(ui.lstClasspath);
            item->setText(txt);
            item->setToolTip(txt);
            ui.lstClasspath->addItem(item);
            _modified = true;
        }
    }
    delete dlg;
}

void DlgProjectDetails::on_actionClassPathRem_triggered() {
    GlobalLogger::instance()->logDebug("Classpath remove...");
    QMessageBox::information(this, "AnnoTool",
                             "Sorry! This feature is not yet implemented :-(");
}

void DlgProjectDetails::on_actionSearchPathAdd_triggered() {
    GlobalLogger::instance()->logDebug("Searchpath add...");
    DlgFileChooser *dlg = new DlgFileChooser(this, true);
    if (dlg->exec() == QDialog::Accepted) {
        QFileInfo file(dlg->selectedPath());
        if (file.isDir()) {
            QString txt = file.filePath();
            QListWidgetItem *item = new QListWidgetItem(ui.lstSearchpath);
            item->setText(txt);
            item->setToolTip(txt);
            ui.lstSearchpath->addItem(item);
            _modified = true;
        }
    }
    delete dlg;
}

void DlgProjectDetails::on_actionSearchPathRem_triggered() {
    GlobalLogger::instance()->logDebug("Searchpath remove...");
    QMessageBox::information(this, "AnnoTool",
                             "Sorry! This feature is not yet implemented :-(");
}

void DlgProjectDetails::accept() {
    GlobalLogger::instance()->logDebug("User accepted project details");
    GlobalProjectManager::instance()->project()->setProjectName(ui.editProjectName->text());
    if (_modified) {
        QMessageBox::information(
            this,
            "AnnoTool",
            "In order to take effect and to prevent data inconsistency\nthe current project should be saved and reopened!");

        ::anno::dt::AnnoProject *prj = GlobalProjectManager::instance()->project();
        prj->classPath()->clear();
        //TODO add listed classes to project!
    }
    QDialog::accept();
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
