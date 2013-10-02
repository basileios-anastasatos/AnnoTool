#include "include/ColorDisplay.h"

#include <QPaintEvent>
#include <QPainter>
#include <QColor>

ColorDisplay::ColorDisplay(QWidget *parent) :
    QWidget(parent), _curBrush(QColor(Qt::white)), _borderPen(QColor(0, 0, 0, 192)) {
    _borderPen.setWidth(1);
}

ColorDisplay::ColorDisplay(const QColor &color, QWidget *parent) :
    QWidget(parent), _curBrush(color), _borderPen(QColor(0, 0, 0, 192)) {
    _borderPen.setWidth(1);
}


void ColorDisplay::paintEvent(QPaintEvent *event) {
    event->accept();

    QPainter painter(this);
    painter.setPen(_borderPen);
    painter.setBrush(_curBrush);
    painter.drawRect(0, 0, width() - 1, height() - 1);
}

void ColorDisplay::setColor(const QColor &color) {
    _curBrush.setColor(color);
    update();
}

QColor ColorDisplay::getColor() const {
    return _curBrush.color();
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
