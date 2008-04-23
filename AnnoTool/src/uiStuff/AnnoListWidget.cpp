#include "include/AnnoListWidget.h"
#include "AnnoFileListModelAdapter.h"
#include "GlobalLogger.h"
#include "GlobalProjectManager.h"

using ::logging::GlobalLogger;
using ::anno::GlobalProjectManager;

AnnoListWidget::AnnoListWidget(QWidget *parent) :
    QDockWidget(parent), _strCount("%1 annotations loaded") {
    ui.setupUi(this);
    setWidget(ui.spacer);
    _model = new AnnoListModelAdapter(this);
    ui.trAnnoList->setModel(_model);
    ui.lbCount->setText(_strCount.arg(0));

    //	connect(ui.lstFiles->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(on_lstFiles_currentRowChanged(
    //					const QModelIndex&, const QModelIndex&)));
}

AnnoListWidget::~AnnoListWidget() {

}

void AnnoListWidget::updateData() {
    _model->update();
    ui.lbCount->setText(_strCount.arg(_model->rowCount(QModelIndex())));
    update();
}

void AnnoListWidget::on_trAnnoList_clicked(const QModelIndex &index) {
    if (_curIndex != index) {
        GlobalLogger::instance()->logDebug("Native selection changed");
        emit 		annoSelectChanged(index.row(), GlobalProjectManager::instance()->selectedFile()->annoList()->at(index.row())->annoId());
        _curIndex = index;
    }
}

void AnnoListWidget::on_trAnnoList_activated(const QModelIndex &index) {
    GlobalLogger::instance()->logDebug("Native activated");
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
