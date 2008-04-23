#ifndef ANNODATAWIDGET_H
#define ANNODATAWIDGET_H

#include <QtGui/QDockWidget>
#include "../gen/ui_AnnoDataWidget.h"
#include <QUuid>

class AnnoDataWidget : public QDockWidget {
        Q_OBJECT

    private:
        Ui::AnnoDataWidgetClass ui;

    public:
        AnnoDataWidget(QWidget *parent = 0);
        ~AnnoDataWidget();

    public slots:
        void annoSelectChanged(int row, QUuid anno);

};

#endif // ANNODATAWIDGET_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
