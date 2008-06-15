#include "include/AnnoFileListWidget.h"
#include <QHeaderView>
#include "GlobalLogger.h"
#include "GlobalProjectManager.h"

using ::logging::GlobalLogger;
using ::anno::GlobalProjectManager;

AnnoFileListWidget::AnnoFileListWidget(QWidget *parent) :
    QDockWidget(parent), _strCount("%1 files loaded") {
    ui.setupUi(this);
    setWidget(ui.spacer);
    _model = new AnnoFileListModelAdapter(this);
    ui.lstFiles->setModel(_model);
    ui.lbCount->setText(_strCount.arg(0));

    connect(ui.lstFiles->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(onLstFilesCurrentRowChanged(
                const QModelIndex &, const QModelIndex &)));

    //	if(ui.lstFiles->horizontalHeader() != NULL)
    //	{
    //		GlobalLogger::instance()->logDebug("configuring list header");
    //		QHeaderView* head = ui.lstFiles->horizontalHeader();
    //		head->setSelectionMode(QAbstractItemView::NoSelection);
    //	}
}

AnnoFileListWidget::~AnnoFileListWidget() {

}

void AnnoFileListWidget::updateData() {
    _model->update();
    ui.lbCount->setText(_strCount.arg(_model->rowCount(QModelIndex())));
    update();
}

void AnnoFileListWidget::onLstFilesCurrentRowChanged(
    const QModelIndex &current, const QModelIndex &previous) {
    GlobalLogger::instance()->logDebug("AnnoFileListWidget::on_lstFiles_currentRowChanged");
    emit annoFileSelectChanged(current.row(), GlobalProjectManager::instance()->getAnnoFile(current.row())->imageUuid());
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
