#ifndef DLGEDITATTRIBUTE_H
#define DLGEDITATTRIBUTE_H

#include <QtGui/QDialog>
#include "../gen/ui_DlgEditAttribute.h"
#include "annoTypesForward.h"

#include <QSet>

class DlgEditAttribute : public QDialog {
        Q_OBJECT

    private:
        Ui::DlgEditAttributeClass ui;
        ::anno::dt::Annotation *_anno;

    private:
        void loadClassList();
        void loadClassAttributes();
        void loadRecentValues();

    private slots:
        void on_cbClass_currentIndexChanged(int index);

    public:
        DlgEditAttribute(::anno::dt::Annotation *anno, QWidget *parent = 0);
        ~DlgEditAttribute();

    public:
        virtual void accept();
        int exec();

    public:
        void setClassMode(bool useClass = true);
        void setEditMode(bool edit = true);
        void setClassName(const QString &className);
        void setClassIndex(int index);
        QString getClassName() const;
        int getClassIndex() const;
        void setAttrName(const QString &name);
        QString getAttrName() const;
        void setValue(const QString &value);
        QString getValue() const;

};

#endif // DLGEDITATTRIBUTE_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
