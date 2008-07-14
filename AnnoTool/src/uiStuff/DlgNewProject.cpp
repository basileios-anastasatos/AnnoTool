#include "include/DlgNewProject.h"
#include <QFileDialog>
#include <QMessageBox>
#include "GlobalConfig.h"

DlgNewProject::DlgNewProject(QWidget *parent) :
    QDialog(parent) {
    ui.setupUi(this);
}

void DlgNewProject::on_btSelProjectPath_clicked() {
    QString
    path =
        QFileDialog::getSaveFileName(
            this,
            tr("Where to store project data?"),
            ".",
            QString("AnnoTool Project (%1)").arg(anno::FileExtensions::asFilter(::anno::GlobalConfig::fileExt.projects)));
    if (!path.isEmpty()) {
        QString end = QString(".%1").arg(::anno::GlobalConfig::fileExt.projects);
        if (!path.endsWith(end, Qt::CaseInsensitive)) {
            path = path.append(end);
        }
        ui.editProjectPath->setText(path);
    }
}

void DlgNewProject::on_btSelAnnoPath_clicked() {
    QString path = QFileDialog::getExistingDirectory(this,
                   tr("Select Default Annotation Folder"));
    ui.editDefaultPath->setText(path);
}

void DlgNewProject::accept() {
    QFileInfo projectPath = projectPathAsFileInfo();
    QFileInfo defaultPath = defaultPathAsFileInfo();
    QString name = projectName();
    if (name.isEmpty() || name.size() < 2) {
        QMessageBox::critical(this, "Invalid Project Name",
                              "Please choose a appropriate project name!");
    } else if (!projectPath.absoluteDir().exists()) {
        QMessageBox::critical(this, "Invalid Project Path",
                              "The given project path seems to be invalid!");
    } else if (!defaultPath.exists() || !defaultPath.isDir()) {
        QMessageBox::critical(this, "Invalid Default Path",
                              "The given Default folder seems to be invalid!");
    } else {
        QString path = projectPath.filePath();
        QString end = QString(".%1").arg(::anno::GlobalConfig::fileExt.projects);
        if (!path.endsWith(end, Qt::CaseInsensitive)) {
            path = path.append(end);
            ui.editProjectPath->setText(path);
        }
        QDialog::accept();
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
