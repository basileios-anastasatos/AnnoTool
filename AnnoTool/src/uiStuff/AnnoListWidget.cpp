#include "include/AnnoListWidget.h"
#include "AnnoFileListModelAdapter.h"
#include "importGlobals.h"

#include <QItemSelectionModel>

AnnoListWidget::AnnoListWidget(QWidget *parent) :
    QDockWidget(parent), _strCount("%1 annotations loaded") {
    ui.setupUi(this);
    setWidget(ui.spacer);
    _model = new AnnoListModelAdapter(this);
    ui.trAnnoList->setModel(_model);
    ui.lbCount->setText(_strCount.arg(0));

    connect(ui.trAnnoList->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(on_trAnnoList_currentRowChanged(
                const QModelIndex &, const QModelIndex &)));

    setStyleSheet("QTreeView::item:selected { border: 1px solid #000000; } QTreeView { selection-color: red; selection-color-background: blue; }");
}

AnnoListWidget::~AnnoListWidget() {
}

void AnnoListWidget::updateData() {
    _model->update();
    ui.lbCount->setText(_strCount.arg(_model->rowCount(QModelIndex())));
    update();
}

void AnnoListWidget::on_trAnnoList_currentRowChanged(const QModelIndex &cur,
        const QModelIndex &prev) {
    if (GlobalProjectManager::instance()->isValid() && GlobalProjectManager::instance()->selectedFile() != NULL) {
        if (cur.isValid() && cur.row() < GlobalProjectManager::instance()->selectedFile()->annoCount()) {
            GlobalLogger::instance()->logDebug("Native selection changed");
            emit annoSelectChanged(cur.row(), GlobalProjectManager::instance()->selectedFile()->getAnnotation(cur.row())->annoId());
        }
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
