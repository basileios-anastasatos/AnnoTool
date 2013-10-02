#ifndef ANNOLISTWIDGET_H
#define ANNOLISTWIDGET_H

#include <QDockWidget>
#include <QUuid>
#include "../gen/ui_AnnoListWidget.h"
#include "AnnoListModelAdapter.h"

class AnnoListWidget : public QDockWidget {
        Q_OBJECT

    private:
        Ui::AnnoListWidgetClass ui;
        AnnoListModelAdapter *_model;
        QString _strCount;

    private slots:
        void onTrAnnoListCurrentRowChanged(const QModelIndex &cur, const QModelIndex &prev);
        void on_actionLockTree_triggered();
        void on_actionDeleteAnnotation_triggered();
        void on_actionDeleteAnnotationDeep_triggered();
        void onModel_dropEventFinished();

    public:
        AnnoListWidget(QWidget *parent = 0);
        ~AnnoListWidget();

    public:
        void updateData();
        void clearData();

    signals:
        void annoSelectChanged(const QModelIndex &index, QUuid anno);

};

#endif // ANNOLISTWIDGET_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
