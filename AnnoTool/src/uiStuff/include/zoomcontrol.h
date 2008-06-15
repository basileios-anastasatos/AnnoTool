#ifndef ZOOMCONTROL_H
#define ZOOMCONTROL_H

#include <QtGui/QWidget>
#include "../gen/ui_zoomcontrol.h"

class ZoomControl : public QWidget {
        Q_OBJECT

    private:
        Ui::ZoomControlClass ui;

    private:
        void initZoomList();

    private slots:
        void on_zoomSlider_valueChanged(int value);
        void on_cbZoom_editTextChanged(const QString &text);

    public:
        ZoomControl(QWidget *parent = 0);
        ~ZoomControl();

    public:
        void setZoom(int value);
        int getZoom();

    signals:
        void zoomChanged(int value);

};

#endif // ZOOMCONTROL_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
