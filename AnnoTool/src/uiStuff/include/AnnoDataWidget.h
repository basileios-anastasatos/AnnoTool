#ifndef ANNODATAWIDGET_H
#define ANNODATAWIDGET_H

#include <QtGui/QDockWidget>
#include "../gen/ui_AnnoDataWidget.h"
#include "AnnoDataModelAdapter.h"
#include "AnnoDataClassesModelAdapter.h"
#include <QUuid>

class AnnoDataWidget : public QDockWidget {
        Q_OBJECT

    private:
        Ui::AnnoDataWidgetClass ui;
        AnnoDataModelAdapter *_modelAttributes;
        AnnoDataClassesModelAdapter *_modelClasses;

    private slots:
        void on_actionAddAttribute_triggered();
        void on_actionRemoveAttribute_triggered();
        void on_actionAddClass_triggered();
        void on_actionRemoveClass_triggered();

        void on_trAttributes_activated(const QModelIndex &index);

    public:
        AnnoDataWidget(QWidget *parent = 0);
        ~AnnoDataWidget();

    public:
        void updateData();
        void clearData();

        void setToolsEnabled(bool enabled = true);

    public slots:
        void annoSelectChanged(int row, QUuid anno);

};

#endif // ANNODATAWIDGET_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
