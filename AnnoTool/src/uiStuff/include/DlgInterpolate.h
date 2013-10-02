#ifndef DLGINTERPOLATE_H
#define DLGINTERPOLATE_H

#include <QDialog>
#include "ui_DlgInterpolate.h"

class DlgInterpolate : public QDialog {
        Q_OBJECT

    public:
        DlgInterpolate(QWidget *parent = 0);
        ~DlgInterpolate();

    private:
        Ui::DlgInterpolateClass ui;
};

#endif // DLGINTERPOLATE_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
