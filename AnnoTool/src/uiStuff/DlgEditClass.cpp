#include "include/DlgEditClass.h"
#include "importGlobals.h"

DlgEditClass::DlgEditClass(QWidget *parent)
    : QDialog(parent) {
    ui.setupUi(this);
}

DlgEditClass::~DlgEditClass() {

}

void DlgEditClass::initClassList() {
    ui.lstClasses->clear();
    if (GlobalProjectManager::instance()->isValid()) {
        const anno::dt::AnnoAvClassList *classes = GlobalProjectManager::instance()->classes();
        QList<QString> names = classes->getClassNames();
        foreach(QString str, names) {
            ui.lstClasses->addItem(str);
        }
    }
}

QString DlgEditClass::getSelectedClassName() const {
    QList<QListWidgetItem *> selItems = ui.lstClasses->selectedItems();
    if(selItems.size() > 0) {
        return selItems[0]->text();
    } else {
        return QString();
    }
}



// vim:ts=4:sts=4:sw=4:tw=80:expandtab
