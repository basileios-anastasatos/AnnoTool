#ifndef COLOREDIT_H
#define COLOREDIT_H

#include <QWidget>
#include "../gen/ui_ColorEdit.h"

#include <QColor>

class QIntValidator;

class ColorEdit: public QWidget {
        Q_OBJECT

    private:
        Ui::ColorEditClass ui;

    private:
        QColor _curColor;
        QIntValidator *_byteValidator;

    private slots:
        void on_btColorChooser_clicked();
        void on_txtRed_editingFinished();
        void on_txtGreen_editingFinished();
        void on_txtBlue_editingFinished();
        void on_txtAlpha_editingFinished();

    public:
        ColorEdit(QWidget *parent = 0);
        ~ColorEdit();

        QColor getColor() const;


    public slots:
        void setColor(const QColor &color);

    signals:
        void colorChanged(QColor color);

};

#endif // COLOREDIT_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
