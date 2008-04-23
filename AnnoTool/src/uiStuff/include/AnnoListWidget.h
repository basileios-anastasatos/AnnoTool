#ifndef ANNOLISTWIDGET_H
#define ANNOLISTWIDGET_H

#include <QtGui/QDockWidget>
#include <QUuid>
#include "../gen/ui_AnnoListWidget.h"
#include "AnnoListModelAdapter.h"

class AnnoListWidget : public QDockWidget {
        Q_OBJECT

    private:
        Ui::AnnoListWidgetClass ui;
        AnnoListModelAdapter *_model;
        QString _strCount;
        QModelIndex _curIndex;

    private slots:
        void on_trAnnoList_clicked(const QModelIndex &index);
        void on_trAnnoList_activated(const QModelIndex &index);

    public:
        AnnoListWidget(QWidget *parent = 0);
        ~AnnoListWidget();

    public:
        void updateData();
        void clearData();

    signals:
        void annoSelectChanged(int row, QUuid anno);

};

#endif // ANNOLISTWIDGET_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
