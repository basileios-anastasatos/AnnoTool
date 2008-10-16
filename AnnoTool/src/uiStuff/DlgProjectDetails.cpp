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
            QList<QFileInfo> *classPath = pm->project()->classPath();
            int size = classPath->size();
            for(int i = 0; i < size; ++i) {
                QString txt = classPath->at(i).filePath();
                QListWidgetItem *item = new QListWidgetItem(ui.lstClasspath);
                item->setText(txt);
                item->setToolTip(txt);
                item->setData(Qt::UserRole, i);
                ui.lstClasspath->addItem(item);
            }
        }
        if (!pm->project()->searchPath()->isEmpty()) {
            QList<QFileInfo> *searchPath = pm->project()->searchPath();
            int size = searchPath->size();
            for(int i = 0; i < size; ++i) {
                QString txt = searchPath->at(i).filePath();
                QListWidgetItem *item = new QListWidgetItem(ui.lstSearchpath);
                item->setText(txt);
                item->setToolTip(txt);
                item->setData(Qt::UserRole, i);
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
            if(dlg->useRelativePath()) {
                file = GlobalProjectManager::instance()->absToRel(file);
            }
            QString txt = file.filePath();
            QListWidgetItem *item = new QListWidgetItem(ui.lstClasspath);
            item->setText(txt);
            item->setToolTip(txt);
            ui.lstClasspath->addItem(item);
            GlobalProjectManager::instance()->project()->addToClassPath(file.filePath());
            _modified = true;
        }
    }
    delete dlg;
}

void DlgProjectDetails::on_actionClassPathRem_triggered() {
    GlobalLogger::instance()->logDebug("Classpath remove...");
//	QMessageBox::information(this, "AnnoTool",
//			"Sorry! This feature is not yet implemented :-(");

    QList<QFileInfo> *classPath = GlobalProjectManager::instance()->project()->classPath();
    QList<QListWidgetItem *> items = ui.lstClasspath->selectedItems();
    if(!items.isEmpty()) {
        for(int i = items.size() - 1; i >= 0; --i) {
            bool ok = false;
            int index = items[i]->data(Qt::UserRole).toInt(&ok);
            if(ok) {
                delete ui.lstClasspath->takeItem(index);
                classPath->removeAt(index);
            }
        }
    }
}

void DlgProjectDetails::on_actionSearchPathAdd_triggered() {
    GlobalLogger::instance()->logDebug("Searchpath add...");
    DlgFileChooser *dlg = new DlgFileChooser(this, true);
    if (dlg->exec() == QDialog::Accepted) {
        QFileInfo file(dlg->selectedPath());
        if (file.isDir()) {
            if(dlg->useRelativePath()) {
                file = GlobalProjectManager::instance()->absToRel(file);
            }
            QString txt = file.filePath();
            QListWidgetItem *item = new QListWidgetItem(ui.lstSearchpath);
            item->setText(txt);
            item->setToolTip(txt);
            ui.lstSearchpath->addItem(item);
            GlobalProjectManager::instance()->project()->addToSearchPath(file.filePath());
            _modified = true;
        }
    }
    delete dlg;
}

void DlgProjectDetails::on_actionSearchPathRem_triggered() {
    GlobalLogger::instance()->logDebug("Searchpath remove...");
//	QMessageBox::information(this, "AnnoTool",
//			"Sorry! This feature is not yet implemented :-(");

    QList<QFileInfo> *searchPath = GlobalProjectManager::instance()->project()->searchPath();
    QList<QListWidgetItem *> items = ui.lstSearchpath->selectedItems();
    if (!items.isEmpty()) {
        for (int i = items.size() - 1; i >= 0; --i) {
            bool ok = false;
            int index = items[i]->data(Qt::UserRole).toInt(&ok);
            if (ok) {
                delete ui.lstSearchpath->takeItem(index);
                searchPath->removeAt(index);
            }
        }
    }
}

void DlgProjectDetails::accept() {
    GlobalLogger::instance()->logDebug("User accepted project details");
    GlobalProjectManager::instance()->project()->setProjectName(ui.editProjectName->text());
    if (_modified) {
        QMessageBox::information(
            this,
            "AnnoTool",
            "In order to take effect and to prevent data inconsistency\nthe current project should be saved and reopened!");
    }
    QDialog::accept();
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
