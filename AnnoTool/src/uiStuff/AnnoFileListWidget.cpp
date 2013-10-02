#include "include/AnnoFileListWidget.h"
#include <QHeaderView>
#include "importGlobals.h"

AnnoFileListWidget::AnnoFileListWidget(QWidget *parent) :
    QDockWidget(parent), _strCount("%1 files loaded") {
    ui.setupUi(this);
    setWidget(ui.spacer);
    _model = new AnnoFileListModelAdapter(this);
    ui.lstFiles->setModel(_model);
    ui.lbCount->setText(_strCount.arg(0));

    connect(ui.lstFiles->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(onLstFilesCurrentRowChanged(
                const QModelIndex &, const QModelIndex &)));
    connect(GlobalProjectManager::instance(), SIGNAL(curAnnoFileSelChanged(int, QUuid, ::anno::dt::AnnoFileData *)), this, SLOT(onPM_annoFileSelectChanged(int, QUuid, ::anno::dt::AnnoFileData *)));

    ui.lstFiles->header()->setStretchLastSection(false);
    ui.lstFiles->setColumnWidth(0, 150);
    ui.lstFiles->setColumnWidth(1, 50);
    ui.lstFiles->header()->setSectionResizeMode(0, QHeaderView::Stretch);
}

AnnoFileListWidget::~AnnoFileListWidget() {

}

void AnnoFileListWidget::updateData() {
    _model->update();
    ui.lbCount->setText(_strCount.arg(_model->rowCount(QModelIndex())));
    update();
}

void AnnoFileListWidget::onLstFilesCurrentRowChanged(const QModelIndex &current,
        const QModelIndex &previous) {
    GlobalLogger::instance()->logDebug("AnnoFileListWidget::on_lstFiles_currentRowChanged");
    emit annoFileSelectChanged(current.row(),
                               GlobalProjectManager::instance()->getAnnoFile(current.row())->imageUuid());
}

void AnnoFileListWidget::onPM_annoFileSelectChanged(int row, QUuid imageId,
        ::anno::dt::AnnoFileData *annoFile) {
    blockSignals(true);
    QModelIndex idx = ui.lstFiles->model()->index(row, 0, QModelIndex());
    ui.lstFiles->setCurrentIndex(idx);
    blockSignals(false);
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
