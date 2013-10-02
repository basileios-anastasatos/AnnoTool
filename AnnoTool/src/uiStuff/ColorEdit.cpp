#include "ColorEdit.h"
#include <QColorDialog>
#include <QIntValidator>
#include <QColor>


ColorEdit::ColorEdit(QWidget *parent)
    : QWidget(parent), _curColor(QColor(Qt::white)), _byteValidator(NULL) {
    ui.setupUi(this);

    _byteValidator = new QIntValidator(0, 255, this);
    ui.txtRed->setValidator(_byteValidator);
    ui.txtGreen->setValidator(_byteValidator);
    ui.txtBlue->setValidator(_byteValidator);
    ui.txtAlpha->setValidator(_byteValidator);
}

ColorEdit::~ColorEdit() {
}

void ColorEdit::on_btColorChooser_clicked() {
    QColor color = QColorDialog::getColor(_curColor, this);
    if(color.isValid()) {
        setColor(color);
    }
}

void ColorEdit::on_txtRed_editingFinished() {
    bool ok = false;
    int tmp = ui.txtRed->text().toInt(&ok, 10);
    if(ok && tmp >= 0 && tmp <= 255) {
        setColor(QColor(tmp, _curColor.green(), _curColor.blue(), _curColor.alpha()));
    }
}

void ColorEdit::on_txtGreen_editingFinished() {
    bool ok = false;
    int tmp = ui.txtGreen->text().toInt(&ok, 10);
    if(ok && tmp >= 0 && tmp <= 255) {
        setColor(QColor(_curColor.red(), tmp, _curColor.blue(), _curColor.alpha()));
    }
}

void ColorEdit::on_txtBlue_editingFinished() {
    bool ok = false;
    int tmp = ui.txtBlue->text().toInt(&ok, 10);
    if(ok && tmp >= 0 && tmp <= 255) {
        setColor(QColor(_curColor.red(), _curColor.green(), tmp, _curColor.alpha()));
    }
}

void ColorEdit::on_txtAlpha_editingFinished() {
    bool ok = false;
    int tmp = ui.txtAlpha->text().toInt(&ok, 10);
    if(ok && tmp >= 0 && tmp <= 255) {
        setColor(QColor(_curColor.red(), _curColor.green(), _curColor.blue(), tmp));
    }
}

QColor ColorEdit::getColor() const {
    return _curColor;
}


void ColorEdit::setColor(const QColor &color) {
    if(color.isValid() && color != _curColor) {
        _curColor = color;
        ui.txtRed->setText(QString::number(color.red(), 10));
        ui.txtGreen->setText(QString::number(color.green(), 10));
        ui.txtBlue->setText(QString::number(color.blue(), 10));
        ui.txtAlpha->setText(QString::number(color.alpha(), 10));
        ui.colorDisplay->setColor(color);

        emit colorChanged(color);
    }
}




// vim:ts=4:sts=4:sw=4:tw=80:expandtab
