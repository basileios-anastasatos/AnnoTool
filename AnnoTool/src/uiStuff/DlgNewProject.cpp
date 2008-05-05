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
    QString path = QFileDialog::getExistingDirectory(this, tr("Select Project Folder"));
    ui.editProjectPath->setText(path);
}

void DlgNewProject::on_btSelAnnoPath_clicked() {
    QString path = QFileDialog::getExistingDirectory(this,
                   tr("Select Default Annotation Folder"));
    ui.editDefaultPath->setText(path);
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

QString DlgNewProject::defaultPath() const {
    return ui.editDefaultPath->text();
}

QFileInfo DlgNewProject::defaultPathAsFileInfo() const {
    return QFileInfo(ui.editDefaultPath->text());
}

void DlgNewProject::setDefaultPath(const QString &path) {
    ui.editDefaultPath->setText(path);
}

void DlgNewProject::setDefaultPath(const QFileInfo &path) {
    ui.editDefaultPath->setText(path.filePath());
}

QString DlgNewProject::projectName() const {
    return ui.editName->text();
}

void DlgNewProject::setProjectName(const QString &name) {
    ui.editName->setText(name);
}

QUuid DlgNewProject::projectUuid() const {
    return QUuid(ui.editUuid->text());
}

void DlgNewProject::setProjectUuid(const QUuid &uuid) {
    ui.editUuid->setText(uuid.toString());
}

void DlgNewProject::accept() {
    QFileInfo projectPath = projectPathAsFileInfo();
    QFileInfo defaultPath = defaultPathAsFileInfo();
    QString name = projectName();
    if (name.isEmpty() || name.size() < 2) {
        QMessageBox::critical(this, "Invalid Project Name",
                              "Please choose a appropriate project name!");
    } else if (!projectPath.exists() || !projectPath.isDir()) {
        QMessageBox::critical(this, "Invalid Project Path",
                              "The given Project folder seems to be invalid!");
    } else if (!defaultPath.exists() || !defaultPath.isDir()) {
        //TODO Update such that relative paths to given project paths are possible
        QMessageBox::critical(this, "Invalid Default Path",
                              "The given Default folder seems to be invalid!");
    } else {
        QDialog::accept();
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
