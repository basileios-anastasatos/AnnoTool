#include "include/dlgnewproject.h"
#include <QFileDialog>
#include <QMessageBox>

DlgNewProject::DlgNewProject(QWidget *parent) :
    QDialog(parent) {
    ui.setupUi(this);
}

DlgNewProject::~DlgNewProject() {

}

void DlgNewProject::on_btSelProjectPath_clicked() {
    QString path = QFileDialog::getExistingDirectory(this,
                   tr("Select Project Folder"));
    ui.editProjectPath->setText(path);
}

void DlgNewProject::on_btSelAnnoPath_clicked() {
    QString path = QFileDialog::getExistingDirectory(this,
                   tr("Select Default Annotation Folder"));
    ui.editAnnoPath->setText(path);
}

QString DlgNewProject::projectPath() const {
    return ui.editProjectPath->text();
}

QFileInfo DlgNewProject::projectPathAsFileInfo() const {
    return QFileInfo(ui.editProjectPath->text());
}

void DlgNewProject::setProjectPath(const QString &path) {
    ui.editProjectPath->setText(path);
}

void DlgNewProject::setProjectPath(const QFileInfo &path) {
    ui.editProjectPath->setText(path.filePath());
}

void DlgNewProject::accept() {
    QFileInfo path(ui.editProjectPath->text());
    if (!path.exists()) {
        QMessageBox::critical(this, "Invalid Data",
                              "The given Project folder seems to be invalid!");
    } else {
        QDialog::accept();
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
