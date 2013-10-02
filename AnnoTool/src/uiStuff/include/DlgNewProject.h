#ifndef DLGNEWPROJECT_H
#define DLGNEWPROJECT_H

#include <QDialog>
#include "../gen/ui_DlgNewProject.h"
#include <QFileInfo>
#include <QUuid>

class DlgNewProject : public QDialog {
        Q_OBJECT

    private:
        Ui::DlgNewProjectClass ui;

    private slots:
        void on_btSelProjectPath_clicked();
        void on_btSelAnnoPath_clicked();

    public:
        DlgNewProject(QWidget *parent = 0);
        ~DlgNewProject();

    public:
        QString projectPath() const;
        QFileInfo projectPathAsFileInfo() const;
        void setProjectPath(const QString &path);
        void setProjectPath(const QFileInfo &path);

        QString defaultPath() const;
        QFileInfo defaultPathAsFileInfo() const;
        void setDefaultPath(const QString &path);
        void setDefaultPath(const QFileInfo &path);

        QString projectName() const;
        void setProjectName(const QString &name);

        QUuid projectUuid() const;
        void setProjectUuid(const QUuid &uuid);

        bool makeDefaultPathRelative() const;
        void setDefaultPathRelative(bool value);

    public slots:
        virtual void accept();

};

// inlining
// ------------------------------------------------------------
inline DlgNewProject::~DlgNewProject() {
}

inline QString DlgNewProject::projectPath() const {
    return ui.editProjectPath->text();
}

inline QFileInfo DlgNewProject::projectPathAsFileInfo() const {
    return QFileInfo(ui.editProjectPath->text());
}

inline void DlgNewProject::setProjectPath(const QString &path) {
    ui.editProjectPath->setText(path);
}

inline void DlgNewProject::setProjectPath(const QFileInfo &path) {
    ui.editProjectPath->setText(path.filePath());
}

inline QString DlgNewProject::defaultPath() const {
    return ui.editDefaultPath->text();
}

inline QFileInfo DlgNewProject::defaultPathAsFileInfo() const {
    return QFileInfo(ui.editDefaultPath->text());
}

inline void DlgNewProject::setDefaultPath(const QString &path) {
    ui.editDefaultPath->setText(path);
}

inline void DlgNewProject::setDefaultPath(const QFileInfo &path) {
    ui.editDefaultPath->setText(path.filePath());
}

inline QString DlgNewProject::projectName() const {
    return ui.editName->text();
}

inline void DlgNewProject::setProjectName(const QString &name) {
    ui.editName->setText(name);
}

inline QUuid DlgNewProject::projectUuid() const {
    return QUuid(ui.editUuid->text());
}

inline void DlgNewProject::setProjectUuid(const QUuid &uuid) {
    ui.editUuid->setText(uuid.toString());
}

inline bool DlgNewProject::makeDefaultPathRelative() const {
    return ui.checkRelative->isChecked();
}

inline void DlgNewProject::setDefaultPathRelative(bool value) {
    ui.checkRelative->setChecked(value);
}
// ------------------------------------------------------------


#endif // DLGNEWPROJECT_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
