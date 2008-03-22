#include "AnnoGraphicsPolygon.h"
#include <QGraphicsSceneMouseEvent>

#include <QMessageBox>
#include <QGraphicsScene>
#include "AnnoGraphicsControlPoint.h"

#include <iostream>

AnnoGraphicsPolygon::AnnoGraphicsPolygon() {
    setupAppearance();
}

AnnoGraphicsPolygon::~AnnoGraphicsPolygon() {
    for (int i = 0; i < _controlPoints.size(); ++i) {
        delete _controlPoints[i];
    }
}

void AnnoGraphicsPolygon::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton &&
            (event->modifiers() & Qt::ShiftModifier) != 0 &&
            (event->modifiers() & Qt::ControlModifier) != 0) {
        //TODO implement remove polygon!
    }
}

void AnnoGraphicsPolygon::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    qreal deltaX = event->pos().x() - event->lastPos().x();
    qreal deltaY = event->pos().y() - event->lastPos().y();
    moveBy(deltaX, deltaY);
}

void AnnoGraphicsPolygon::validateCP() {
    for (int i = 0; i < _controlPoints.size(); ++i) {
        _controlPoints[i]->setIndex(i);
    }
}

void AnnoGraphicsPolygon::initControlPoints() {
    QPolygonF poly = polygon();
    for (int i = 0; i < poly.size(); ++i) {
        AnnoGraphicsControlPoint *cp = new AnnoGraphicsControlPoint(this, i);
        cp->setZValue(20);
        cp->setParentItem(this);
        _controlPoints << cp;
    }
}

void AnnoGraphicsPolygon::insertControlPoint(int index,
        AnnoGraphicsControlPoint *cp) {
    QPolygonF p = polygon();
    QPointF point = p[index];
    p.insert(index, point);
    setPolygon(p);

    _controlPoints.insert(index, cp);
    validateCP();
}
void AnnoGraphicsPolygon::removeControlPoint(int index) {
    QPolygonF p = polygon();
    p.remove(index);
    setPolygon(p);

    AnnoGraphicsControlPoint *cp = _controlPoints[index];
    _controlPoints.remove(index);
    delete cp;
    validateCP();
}

void AnnoGraphicsPolygon::moveControlPoint(int index, qreal x, qreal y) {
    if (index < 0 || index >= _controlPoints.size()) {
        std::cerr << "INVALID moveControlPoint: " << index << "["
                  << _controlPoints.size() << "]" << std::endl;
        throw QString("Invalid Index");
    }

    AnnoGraphicsControlPoint *cp = _controlPoints[index];
    cp->moveBy(x, y);

    QPolygonF poly = polygon();
    QPointF &point = poly[index];
    point.setX(point.x() + x);
    point.setY(point.y() + y);
    setPolygon(poly);
}

void AnnoGraphicsPolygon::setupAppearance() {
    setVisible(true);
    QPen pen(QColor(30, 30, 255, 255));
    pen.setWidth(1);
    setPen(pen);
    setBrush(QBrush(QColor(255, 255, 60, 45)));
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
