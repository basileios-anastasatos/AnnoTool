#include "include/AnnoListWidget.h"
#include "AnnoFileListModelAdapter.h"
#include "AnnoOperationHelper.h"
#include "AnnoToolMainWindow.h"
#include "importGlobals.h"
#include <QItemSelectionModel>

AnnoListWidget::AnnoListWidget(QWidget *parent) :
    QDockWidget(parent), _strCount("%1 annotations loaded") {
    ui.setupUi(this);
    setWidget(ui.spacer);
    _model = new AnnoListModelAdapter(this);
    ui.trAnnoList->setModel(_model);
    ui.lbCount->setText(_strCount.arg(0));

    ui.btLockTree->setDefaultAction(ui.actionLockTree);
    ui.btDelAnno->setDefaultAction(ui.actionDeleteAnnotation);
    ui.btDelAnnoDeep->setDefaultAction(ui.actionDeleteAnnotationDeep);

    connect(ui.trAnnoList->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(onTrAnnoListCurrentRowChanged(
                const QModelIndex &, const QModelIndex &)));
    connect(_model, SIGNAL(dropEventFinished()), this, SLOT(onModel_dropEventFinished()));

    ui.trAnnoList->setColumnWidth(0, 265);

    //setStyleSheet("QTreeView::item:selected { border: 1px solid #000000; } QTreeView { selection-color: red; selection-color-background: blue; }");
}

AnnoListWidget::~AnnoListWidget() {
}

void AnnoListWidget::updateData() {
    int annoCount = 0;
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if (pm->isValid()) {
        const anno::dt::AnnoFileData *curFile = pm->selectedFile();
        if(curFile != NULL) {
            annoCount = curFile->annoCount();
        }
    }

    ui.lbCount->setText(_strCount.arg(annoCount));
    _model->update();
    update();
    ui.trAnnoList->expandAll();
}

void AnnoListWidget::onTrAnnoListCurrentRowChanged(const QModelIndex &cur,
        const QModelIndex &prev) {
    if (GlobalProjectManager::instance()->isValid() && GlobalProjectManager::instance()->selectedFile() != NULL) {
        if (cur.isValid() && cur.row() < GlobalProjectManager::instance()->selectedFile()->annoCount()) {
            emit annoSelectChanged(cur, GlobalProjectManager::instance()->selectedFile()->getAnnotation(cur.internalId())->annoId());
        }
    }
}

void AnnoListWidget::on_actionLockTree_triggered() {
    ui.trAnnoList->setItemsExpandable(!ui.actionLockTree->isChecked());
    ui.trAnnoList->expandAll();
}

void AnnoListWidget::on_actionDeleteAnnotation_triggered() {
    if (GlobalProjectManager::instance()->isValid()) {
        anno::dt::AnnoFileData *annoFile = GlobalProjectManager::instance()->selectedFile();
        if(annoFile != NULL) {
            anno::dt::Annotation *selAnno = GlobalProjectManager::instance()->selectedAnno();
            if(selAnno != NULL) {
                anno::dt::AnnoOperationHelper annoOp(annoFile);
                annoOp.deleteAnnotation(selAnno, false);
                AnnoToolMainWindow::updateUI();
            }
        }
    }
}

void AnnoListWidget::on_actionDeleteAnnotationDeep_triggered() {
    if (GlobalProjectManager::instance()->isValid()) {
        anno::dt::AnnoFileData *annoFile = GlobalProjectManager::instance()->selectedFile();
        if(annoFile != NULL) {
            anno::dt::Annotation *selAnno = GlobalProjectManager::instance()->selectedAnno();
            if(selAnno != NULL) {
                anno::dt::AnnoOperationHelper annoOp(annoFile);
                annoOp.deleteAnnotation(selAnno, true);
                AnnoToolMainWindow::updateUI();
            }
        }
    }
}

void AnnoListWidget::onModel_dropEventFinished() {
    ui.trAnnoList->expandAll();
}




// vim:ts=4:sts=4:sw=4:tw=80:expandtab
