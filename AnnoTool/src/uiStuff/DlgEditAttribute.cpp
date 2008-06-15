#include "include/DlgEditAttribute.h"
#include "importGlobals.h"
#include "Annotation.h"
#include "AnnoClassDefinition.h"
#include "AnnoAvClassList.h"

#include <QLineEdit>

QSet<QString> DlgEditAttribute::_acAttributes;
QSet<QString> DlgEditAttribute::_acValues;

DlgEditAttribute::DlgEditAttribute(::anno::dt::Annotation *anno, QWidget *parent) :
    QDialog(parent), _anno(anno) {
    setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
    ui.setupUi(this);

    loadClassList();
    loadRecentValues();
    ui.cbValue->lineEdit()->setText(QString());
}

DlgEditAttribute::~DlgEditAttribute() {
}

void DlgEditAttribute::loadClassList() {
    if (_anno != NULL) {
        ui.cbClass->addItem(QString("< No Class >"));
        QList<QString> clst = _anno->classes();
        QListIterator<QString> i(clst);
        while (i.hasNext()) {
            QString cur = i.next();
            ui.cbClass->addItem(cur);
        }
    }
}

void DlgEditAttribute::loadClassAttributes() {
    int index = ui.cbClass->currentIndex();
    QString cName = ui.cbClass->currentText();
    ui.cbName->clear();
    if (index >= 1 && GlobalProjectManager::instance()->isValid()) {
        ::anno::dt::AnnoAvClassList *classList = GlobalProjectManager::instance()->classes();
        ::anno::dt::AnnoClassDefinition *classDef = classList->getClass(cName);
        if (classDef != NULL) {
            QList<QString> attrList;
            classDef->allAttributes(attrList);
            QListIterator<QString> i(attrList);
            while (i.hasNext()) {
                QString cur = i.next();
                ui.cbName->addItem(cur);
            }
        }
    }
}

void DlgEditAttribute::loadRecentValues() {
    QSetIterator<QString> i(_acValues);
    while (i.hasNext()) {
        ui.cbValue->insertItem(i.next());
    }
}

void DlgEditAttribute::accept() {
    if (ui.cbName->isEditable()) {
        QString curTxt = ui.cbName->currentText();
        if(!curTxt.isEmpty()) {
            _acAttributes.insert(curTxt);
        }
    }

    QString curTxt = ui.cbValue->currentText();
    if(!curTxt.isEmpty()) {
        _acValues.insert(curTxt);
    }

    QDialog::accept();
}

int DlgEditAttribute::exec() {
    setFocus();
    return QDialog::exec();
}

void DlgEditAttribute::setClassMode(bool useClass) {
    ui.cbName->setEditable(!useClass);

    if (!useClass) {
        QSetIterator<QString> i(_acAttributes);
        while (i.hasNext()) {
            ui.cbName->insertItem(i.next());
        }
        ui.cbName->lineEdit()->setText(QString());
    }
}

void DlgEditAttribute::setEditMode(bool edit) {
    ui.cbClass->setEnabled(!edit);
    ui.cbName->setEnabled(!edit);
}

void DlgEditAttribute::on_cbClass_currentIndexChanged(int index) {
    loadClassAttributes();
    setClassMode(index >= 1);
}

void DlgEditAttribute::setClassName(const QString &className) {
    for (int i = 0; i < ui.cbClass->count(); ++i) {
        QString cur = ui.cbClass->itemText(i);
        if (cur == className) {
            ui.cbClass->setCurrentIndex(i);
            break;
        }
    }
}

void DlgEditAttribute::setClassIndex(int index) {
    if (index <= ui.cbClass->count()) {
        ui.cbClass->setCurrentIndex(index);
    }
}

QString DlgEditAttribute::getClassName() const {
    if (ui.cbClass->currentIndex() >= 1) {
        return ui.cbClass->currentText();
    }
    return QString();
}

int DlgEditAttribute::getClassIndex() const {
    return ui.cbClass->currentIndex();
}

void DlgEditAttribute::setAttrName(const QString &name) {
    if (ui.cbClass->currentIndex() == 0) {
        ui.cbName->lineEdit()->setText(name);
    } else {
        for (int i = 0; i < ui.cbName->count(); ++i) {
            QString cur = ui.cbName->itemText(i);
            if (cur == name) {
                ui.cbName->setCurrentIndex(i);
                break;
            }
        }
    }
}

QString DlgEditAttribute::getAttrName() const {
    return ui.cbName->currentText();
}

void DlgEditAttribute::setValue(const QString &value) {
    ui.cbValue->lineEdit()->setText(value);
}

QString DlgEditAttribute::getValue() const {
    return ui.cbValue->currentText();
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
