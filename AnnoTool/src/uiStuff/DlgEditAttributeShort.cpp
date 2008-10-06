#include "include/DlgEditAttributeShort.h"
#include "importGlobals.h"

#include <QLineEdit>

DlgEditAttributeShort::DlgEditAttributeShort(QWidget *parent)
    : QDialog(parent) {
    setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
    ui.setupUi(this);

    loadRecentValues();
    ui.cbValue->lineEdit()->setText(QString());
}

DlgEditAttributeShort::~DlgEditAttributeShort() {

}

void DlgEditAttributeShort::loadRecentValues() {
    QSetIterator<QString> i(GlobalToolManager::instance()->recentValues()->attrValues);
    while (i.hasNext()) {
        ui.cbValue->addItem(i.next());
    }
}

void DlgEditAttributeShort::accept() {
    if (ui.cbName->isEditable()) {
        QString curTxt = ui.cbName->currentText();
        if(!curTxt.isEmpty()) {
            GlobalToolManager::instance()->recentValues()->attrNames.insert(curTxt);
        }
    }

    QString curTxt = ui.cbValue->currentText();
    if(!curTxt.isEmpty()) {
        GlobalToolManager::instance()->recentValues()->attrValues.insert(curTxt);
    }

    QDialog::accept();
}

int DlgEditAttributeShort::exec() {
    setFocus();
    return QDialog::exec();
}

void DlgEditAttributeShort::setNameEdit(bool edit) {
    ui.cbName->setEnabled(edit);
}

void DlgEditAttributeShort::setAttrName(const QString &name) {
    ui.cbName->lineEdit()->setText(name);
}

QString DlgEditAttributeShort::getAttrName() const {
    return ui.cbName->currentText();
}

void DlgEditAttributeShort::setValue(const QString &value) {
    ui.cbValue->lineEdit()->setText(value);
}

QString DlgEditAttributeShort::getValue() const {
    return ui.cbValue->currentText();
}



// vim:ts=4:sts=4:sw=4:tw=80:expandtab
