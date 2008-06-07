#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QtGui/QDialog>
#include "../gen/ui_DlgAbout.h"

class DlgAbout : public QDialog {
        Q_OBJECT

    private:
        Ui::DlgAboutClass ui;

    public:
        DlgAbout(QWidget *parent = 0);
        ~DlgAbout();

};

#endif // DLGABOUT_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
