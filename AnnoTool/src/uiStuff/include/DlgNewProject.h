#ifndef DLGNEWPROJECT_H
#define DLGNEWPROJECT_H

#include <QtGui/QDialog>
#include "../gen/ui_dlgnewproject.h"
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

    public slots:
        virtual void accept();

};

#endif // DLGNEWPROJECT_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
