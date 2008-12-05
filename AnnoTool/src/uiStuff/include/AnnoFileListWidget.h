#ifndef ANNOFILELISTWIDGET_H
#define ANNOFILELISTWIDGET_H

#include <QtGui/QDockWidget>
#include <QUuid>
#include <QMenu>
#include "../gen/ui_AnnoFileListWidget.h"
#include "AnnoFileListModelAdapter.h"
#include "annoTypesForward.h"


class AnnoFileListWidget : public QDockWidget {
        Q_OBJECT

    private:
        Ui::AnnoFileListWidgetClass ui;
        AnnoFileListModelAdapter *_model;
        QString _strCount;
        QMenu _contextMenu;

    private slots:
        void onLstFilesCurrentRowChanged(const QModelIndex &current,
                                         const QModelIndex &previous);
        void onPM_annoFileSelectChanged(int row, QUuid imageId, ::anno::dt::AnnoFileData *annoFile);

    public:
        AnnoFileListWidget(QWidget *parent = 0);
        ~AnnoFileListWidget();

    public:
        void updateData();
        void clearData();

    signals:
        void annoFileSelectChanged(int row, QUuid image);

};

#endif // ANNOFILELISTWIDGET_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
