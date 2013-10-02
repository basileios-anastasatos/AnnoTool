#ifndef DLGEDITATTRIBUTESHORT_H
#define DLGEDITATTRIBUTESHORT_H

#include <QDialog>
#include "ui_DlgEditAttributeShort.h"

class DlgEditAttributeShort: public QDialog {
        Q_OBJECT

    private:
        Ui::DlgEditAttributeShortClass ui;

    private:
        void loadRecentValues();

    public:
        DlgEditAttributeShort(QWidget *parent = 0);
        ~DlgEditAttributeShort();

    public:
        virtual void accept();
        int exec();

    public:
        void setNameEdit(bool edit = false);
        void setAttrName(const QString &name);
        QString getAttrName() const;
        void setValue(const QString &value);
        QString getValue() const;

};

#endif // DLGEDITATTRIBUTESHORT_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
