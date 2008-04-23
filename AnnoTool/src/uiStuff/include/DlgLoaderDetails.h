#ifndef DLGLOADERDETAILS_H
#define DLGLOADERDETAILS_H

#include <QtGui/QDialog>
#include "ui_DlgLoaderDetails.h"

class DlgLoaderDetails : public QDialog {
        Q_OBJECT

    private:
        Ui::DlgLoaderDetailsClass ui;

    private:
        void initLoaderData();
        static QString listToString(const QString &sep, const QList<QString> &lst);

    private slots:
        void on_cbLoader_currentIndexChanged(int index);

    public:
        DlgLoaderDetails(QWidget *parent = 0);
        ~DlgLoaderDetails();

};

#endif // DLGLOADERDETAILS_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
