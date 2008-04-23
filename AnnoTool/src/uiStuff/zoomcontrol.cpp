#include "include/zoomcontrol.h"

ZoomControl::ZoomControl(QWidget *parent) :
    QWidget(parent) {
    ui.setupUi(this);
}

ZoomControl::~ZoomControl() {
}

void ZoomControl::on_zoomSlider_valueChanged(int value) {
    ui.editZoom->setText(QString("%1").arg(value));
    emit zoomChanged(value);
}

void ZoomControl::on_editZoom_textEdited(const QString &text) {
    bool isOk = false;
    int value = text.toInt(&isOk, 10);
    if(isOk && value >= 10 && value <= 500) {
        ui.zoomSlider->setValue(value);
    } else if(!isOk) {
        on_zoomSlider_valueChanged(ui.zoomSlider->value());
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
