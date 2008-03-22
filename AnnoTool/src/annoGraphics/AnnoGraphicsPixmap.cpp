#include "AnnoGraphicsPixmap.h"
#include "AnnoGraphicsPolygon.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>
#include <iostream>

AnnoGraphicsPixmap::AnnoGraphicsPixmap(const QPixmap &pixmap,
                                       QGraphicsItem *parent) :
    QGraphicsPixmapItem(pixmap, parent) {
}

AnnoGraphicsPixmap::~AnnoGraphicsPixmap() {
    for(int i = 0; i < _polygons.size(); ++i) {
        delete _polygons[i];
    }
}

void AnnoGraphicsPixmap::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton &&
            (event->modifiers() & Qt::ShiftModifier) != 0 &&
            (event->modifiers() & Qt::ControlModifier) != 0) {
        AnnoGraphicsPolygon *poly = new AnnoGraphicsPolygon();
        QPolygonF p;
        p << event->pos();
        poly->setPolygon(p);
        poly->setZValue(20);
        poly->setParentItem(this);
        poly->initControlPoints();
        _polygons << poly;
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
