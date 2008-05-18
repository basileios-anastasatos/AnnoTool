#include "include/DlgImporter.h"
#include "importGlobals.h"
#include <QMessageBox>

DlgImporter::DlgImporter(QWidget *parent) :
    QDialog(parent) {
    ui.setupUi(this);
    initList();
}

DlgImporter::~DlgImporter() {

}

void DlgImporter::initList() {
    GlobalImExportManager *ieman = GlobalImExportManager::instance();
    ui.cbPluginName->addItem(QString("< No Importer Selected >"));
    int count = ieman->importerCount();
    for (int i = 0; i < count; ++i) {
        anno::ImporterPlugin *p = ieman->getImporter(i);
        if (p != NULL) {
            ui.cbPluginName->addItem(p->name());
        }
    }
}

void DlgImporter::on_cbPluginName_currentIndexChanged(int index) {
    if (index >= 1) {
        anno::ImporterPlugin *p = GlobalImExportManager::instance()->getImporter(index - 1);
        if (p != NULL) {
            ui.editAuthor->setText(p->author());
            ui.txtDesc->setText(p->description());
        }
    }
}

int DlgImporter::getSelectedImporter() const {
    return ui.cbPluginName->currentIndex() - 1;
}

void DlgImporter::setSelectedImporter(int index) {
    if (index >= 0 && index < ui.cbPluginName->count()) {
        ui.cbPluginName->setCurrentIndex(index);
    }
}

anno::ImporterPlugin *DlgImporter::getSelectedImporterPtr() const {
    anno::ImporterPlugin *p = GlobalImExportManager::instance()->getImporter(getSelectedImporter());
    return p;
}

void DlgImporter::accept() {
    if (ui.cbPluginName->currentIndex() <= 0) {
        QMessageBox::critical( this, "AnnoTool", "Please select an importer plugin.");
    } else {
        QDialog::accept();
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
