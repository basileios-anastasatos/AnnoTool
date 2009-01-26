#include <iostream>

#include "include/AnnoGraphicsView.h"
#include "AnnoGLView.h"
#include "importGlobals.h"

AnnoGraphicsView::AnnoGraphicsView(QWidget *parent) :
    QGraphicsView(parent), _openGl(false) {
    setupView();
}

AnnoGraphicsView::AnnoGraphicsView(QGraphicsScene *scene, QWidget *parent) :
    QGraphicsView(scene, parent), _openGl(false) {
    setupView();
}

AnnoGraphicsView::~AnnoGraphicsView() {
}

void AnnoGraphicsView::setupView() {
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setupBack();

    if(GlobalConfig::instance()->getInt("graphics.useGL", 0)) {
        int samples = GlobalConfig::instance()->getInt("graphics.glSampling", 4);
        AnnoGLView::_glFormat.setSamples(samples);
        AnnoGLView *glView = new AnnoGLView(this);
        setViewport(glView);
        GlobalLogger::instance()->logInfo(QString("Using OpenGL rendering engine with %1x Multisampling.").arg(glView->format().samples()));
        _openGl = true;
    } else {
        GlobalLogger::instance()->logInfo("Using Qt rendering engine.");
    }
}

void AnnoGraphicsView::setupBack() {
    QImage img(60, 60, QImage::Format_ARGB32);
    QPainter painter(&img);
    QPen pen = QPen(QColor(0, 0, 0, 0));
    pen.setWidth(0);
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(210, 250, 210, 255)));
    painter.drawRect(0, 0, 60, 60);
    painter.setBrush(QBrush(QColor(200, 220, 200, 255)));
    painter.drawRect(0, 0, 30, 30);
    painter.drawRect(30, 30, 30, 30);

    setBackgroundBrush(QBrush(img));
}

void AnnoGraphicsView::drawBackground(QPainter *painter, const QRectF &rect) {
    painter->resetTransform();
    painter->setBrush(backgroundBrush());
    painter->drawRect(0, 0, width(), height());
}

void AnnoGraphicsView::wheelEvent(QWheelEvent *event) {
    event->ignore();
}

void AnnoGraphicsView::keyReleaseEvent(QKeyEvent *event) {
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    assert(pm != NULL);

    if (event->key() == Qt::Key_Up) {
        pm->setSelectedFileRow(pm->selectedFileRow() - 1);
    } else if (event->key() == Qt::Key_Down) {
        pm->setSelectedFileRow(pm->selectedFileRow() + 1);
    } else {
        QGraphicsView::keyReleaseEvent(event);
    }

}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
