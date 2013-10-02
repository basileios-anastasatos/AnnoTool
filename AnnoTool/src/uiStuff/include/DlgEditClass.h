#ifndef DLGEDITCLASS_H
#define DLGEDITCLASS_H

#include <QDialog>
#include "ui_DlgEditClass.h"

class DlgEditClass: public QDialog {
        Q_OBJECT

    private:
        Ui::DlgEditClassClass ui;

    public:
        DlgEditClass(QWidget *parent = 0)		;
        ~DlgEditClass();

    public:
        void initClassList();
        QString getSelectedClassName() const;


};

#endif // DLGEDITCLASS_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
