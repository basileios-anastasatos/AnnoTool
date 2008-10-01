#include "include/AnnoDataWidget.h"
#include "DlgEditAttribute.h"
#include "DlgEditClass.h"
#include "Annotation.h"
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
    ::anno::dt::Annotation *curAnno = GlobalProjectManager::instance()->selectedAnno();
    if (curAnno != NULL) {
        DlgEditAttribute *dlg = new DlgEditAttribute(curAnno, this);
        if (dlg->exec() == QDialog::Accepted) {
            anno::dt::AnnoAttribute attr(curAnno);
            attr.setName(dlg->getAttrName());
            attr.setClassName(dlg->getClassName());
            attr.setValue(dlg->getValue());
            curAnno->addAttribute(attr);
            updateListData();
        }
        delete dlg;
    }
}

void AnnoDataWidget::on_actionRemoveAttribute_triggered() {
    GlobalLogger::instance()->logDebug("AnnoDataWidget: remove attribute");
    anno::dt::Annotation *curAnno = GlobalProjectManager::instance()->selectedAnno();
    QModelIndex index = ui.trAttributes->selectionModel()->currentIndex();
    if (curAnno != NULL) {
        curAnno->removeAttribute(index.row());
        updateListData();
    }
}

void AnnoDataWidget::on_actionAddClass_triggered() {
    GlobalLogger::instance()->logDebug("AnnoDataWidget: add class");
    ::anno::dt::Annotation *curAnno = GlobalProjectManager::instance()->selectedAnno();
    if (curAnno != NULL) {
        DlgEditClass *dlg = new DlgEditClass(this);
        dlg->initClassList();
        if (dlg->exec() == QDialog::Accepted) {
            QString className = dlg->getSelectedClassName();
            if(!className.isEmpty()) {
                QList<QString> usedClasses = curAnno->classes();
                if(!usedClasses.contains(className)) {
                    curAnno->addClass(className);
                    updateListData();
                }
            }
        }
        delete dlg;
    }
}

void AnnoDataWidget::on_actionRemoveClass_triggered() {
    GlobalLogger::instance()->logDebug("AnnoDataWidget: remove class");
    anno::dt::Annotation *curAnno = GlobalProjectManager::instance()->selectedAnno();
    int selIndex = ui.lstClasses->selectionModel()->currentIndex().row();
    if (curAnno != NULL && selIndex >= 0) {
        curAnno->removeClass(curAnno->classes()[selIndex]);
        updateListData();
    }
}

void AnnoDataWidget::on_trAttributes_activated(const QModelIndex &index) {
    anno::dt::Annotation *curAnno = GlobalProjectManager::instance()->selectedAnno();
    if (curAnno != NULL) {
        anno::dt::AnnoAttribute *attr = curAnno->getAttribute(index.row());
        if (attr != NULL) {
            DlgEditAttribute *dlg = new DlgEditAttribute(curAnno, this);
            dlg->setClassName(attr->className());
            dlg->setAttrName(attr->name());
            dlg->setValue(attr->value());
            dlg->setEditMode(true);
            if (dlg->exec() == QDialog::Accepted) {
                attr->setValue(dlg->getValue());
            }
            delete dlg;
        }
    }
}

void AnnoDataWidget::updateAllData() {
    updateListData();
    updateShapeInfo();
}

void AnnoDataWidget::updateListData() {
    _modelAttributes->update();
    _modelClasses->update();
}

void AnnoDataWidget::updateShapeInfo() {
    ::anno::dt::Annotation *curAnno = GlobalProjectManager::instance()->selectedAnno();
    if (curAnno != NULL) {
        ui.lbInfo->setText(curAnno->annoInfo());
    } else {
        ui.lbInfo->setText("--");
    }
}

void AnnoDataWidget::setToolsEnabled(bool enabled) {
    ui.actionAddAttribute->setEnabled(enabled);
    ui.actionRemoveAttribute->setEnabled(enabled);
    ui.actionAddClass->setEnabled(enabled);
    ui.actionRemoveClass->setEnabled(enabled);
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
