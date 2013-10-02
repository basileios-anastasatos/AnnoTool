#ifndef DLGFILECHOOSER_H
#define DLGFILECHOOSER_H

#include <QDialog>
#include "../gen/ui_DlgFileChooser.h"

class DlgFileChooser : public QDialog {
        Q_OBJECT

    private:
        Ui::DlgFileChooserClass ui;
        bool _selFolder;
        QString _filters;

    private slots:
        void on_btSelect_clicked();

    public:
        DlgFileChooser(QWidget *parent = 0, bool selFolder = false);
        ~DlgFileChooser();
        void setSelectFilter(const QString &filters);
        void setRelativePath(bool val);
        QString selectedPath() const;
        bool selectedDir() const;
        bool selectedFile() const;
        bool useRelativePath() const;

    public slots:
        virtual void accept();

};

#endif // DLGFILECHOOSER_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
