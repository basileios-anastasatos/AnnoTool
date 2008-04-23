#ifndef DLGADDIMAGE_H
#define DLGADDIMAGE_H

#include <QtGui/QDialog>
#include "../gen/ui_DlgAddImage.h"

class DlgAddImage : public QDialog {
        Q_OBJECT

    private:
        Ui::DlgAddImageClass ui;
        QString _strCount;
        QList<QString> _imageList;

    private:
        void initAnnoPath();

    private slots:
        void on_btSelPath_clicked();
        void on_btAddImages_clicked();

    public:
        DlgAddImage(QWidget *parent = 0);
        ~DlgAddImage();

    public:
        QList<QString> images() const;
        bool saveImagesRel() const;
        QString annoSavePath() const;

    public slots:
        virtual void accept();

};

#endif // DLGADDIMAGE_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
