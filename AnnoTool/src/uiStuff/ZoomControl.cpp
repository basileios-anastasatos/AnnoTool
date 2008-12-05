#include "include/ZoomControl.h"
#include <QLineEdit>

ZoomControl::ZoomControl(QWidget *parent) :
    QWidget(parent), _sliderActive(false) {
    ui.setupUi(this);
    initZoomList();
}

ZoomControl::~ZoomControl() {
}

void ZoomControl::initZoomList() {
    ui.cbZoom->addItem("10");
    ui.cbZoom->addItem("25");
    ui.cbZoom->addItem("50");
    ui.cbZoom->addItem("75");
    ui.cbZoom->addItem("100");
    ui.cbZoom->addItem("150");
    ui.cbZoom->addItem("200");
    ui.cbZoom->addItem("300");
    ui.cbZoom->addItem("400");
    ui.cbZoom->addItem("500");
}

void ZoomControl::on_zoomSlider_valueChanged(int value) {
    ui.cbZoom->lineEdit()->setText(QString("%1").arg(value));
    emit zoomChanged(value);
}

void ZoomControl::on_zoomSlider_sliderMoved(int value) {
    ui.cbZoom->lineEdit()->setText(QString("%1").arg(value));
}

void ZoomControl::on_zoomSlider_sliderPressed() {
    _sliderActive = true;
}

void ZoomControl::on_zoomSlider_sliderReleased() {
    _sliderActive = false;
}

void ZoomControl::on_cbZoom_editTextChanged(const QString &text) {
    if(!_sliderActive) {
        bool isOk = false;
        int value = text.toInt(&isOk, 10);
        if (isOk && value >= 10 && value <= 500) {
            ui.zoomSlider->setValue(value);
        } else if (!isOk) {
            on_zoomSlider_valueChanged(ui.zoomSlider->value());
        }
    }
}

void ZoomControl::setZoom(int value) {
    ui.zoomSlider->setValue(value);
}

int ZoomControl::getZoom() {
    return ui.zoomSlider->value();
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
