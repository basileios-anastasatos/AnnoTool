#ifndef ZOOMCONTROL_H
#define ZOOMCONTROL_H

#include <QWidget>
#include "../gen/ui_ZoomControl.h"

class ZoomControl : public QWidget {
        Q_OBJECT

    private:
        Ui::ZoomControlClass ui;
        bool _sliderActive;

    private:
        void initZoomList();

    private slots:
        void on_zoomSlider_valueChanged(int value);
        void on_zoomSlider_sliderMoved(int value);
        void on_zoomSlider_sliderPressed();
        void on_zoomSlider_sliderReleased();
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
