#ifndef DLGPROJECTDETAILS_H
#define DLGPROJECTDETAILS_H

#include <QtGui/QDialog>
#include "../gen/ui_DlgProjectDetails.h"

class DlgProjectDetails : public QDialog {
        Q_OBJECT

    private:
        Ui::DlgProjectDetailsClass ui;
        bool _modified;

    private slots:
        void on_actionClassPathAdd_triggered();
        void on_actionClassPathRem_triggered();
        void on_actionSearchPathAdd_triggered();
        void on_actionSearchPathRem_triggered();

    private:
        void setupActions();
        void initData();

    public:
        DlgProjectDetails(QWidget *parent = 0);
        ~DlgProjectDetails();

    public slots:
        virtual void accept();

};

#endif // DLGPROJECTDETAILS_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
