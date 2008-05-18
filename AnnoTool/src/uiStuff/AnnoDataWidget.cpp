#include "include/AnnoDataWidget.h"
#include "importGlobals.h"

AnnoDataWidget::AnnoDataWidget(QWidget *parent) :
    QDockWidget(parent) {
    ui.setupUi(this);
    setWidget(ui.spacer);
    _modelAttributes = new AnnoDataModelAdapter(this);
    _modelClasses = new AnnoDataClassesModelAdapter(this);
    ui.trAttributes->setModel(_modelAttributes);
    ui.lstClasses->setModel(_modelClasses);

    ui.btAttrAdd->setDefaultAction(ui.actionAddAttribute);
    ui.btAttrRem->setDefaultAction(ui.actionRemoveAttribute);
    ui.btClassAdd->setDefaultAction(ui.actionAddClass);
    ui.btClassRem->setDefaultAction(ui.actionRemoveClass);
}

AnnoDataWidget::~AnnoDataWidget() {

}

void AnnoDataWidget::on_actionAddAttribute_triggered() {
    GlobalLogger::instance()->logDebug("AnnoDataWidget: add attribute");
}

void AnnoDataWidget::on_actionRemoveAttribute_triggered() {
    GlobalLogger::instance()->logDebug("AnnoDataWidget: remove attribute");
}

void AnnoDataWidget::on_actionAddClass_triggered() {
    GlobalLogger::instance()->logDebug("AnnoDataWidget: add class");
}

void AnnoDataWidget::on_actionRemoveClass_triggered() {
    GlobalLogger::instance()->logDebug("AnnoDataWidget: remove class");
}

void AnnoDataWidget::on_trAttributes_activated(const QModelIndex &index) {
    anno::dt::AnnoAttribute attr = GlobalProjectManager::instance()->selectedAnno()->attributes()->at(index.row());
    GlobalLogger::instance()->logDebug(QString("AnnoDataWidget: activated %1").arg(attr.name));
}

void AnnoDataWidget::updateData() {
    _modelAttributes->update();
    _modelClasses->update();
    //ui.lbCount->setText(_strCount.arg(_model->rowCount(QModelIndex())));
    update();
}

void AnnoDataWidget::annoSelectChanged(int row, QUuid anno) {
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
