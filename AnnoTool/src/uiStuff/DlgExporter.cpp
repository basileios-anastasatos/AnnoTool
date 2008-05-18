#include "include/DlgExporter.h"
#include "importGlobals.h"
#include <QMessageBox>

DlgExporter::DlgExporter(QWidget *parent) :
    QDialog(parent) {
    ui.setupUi(this);
    initList();
}

DlgExporter::~DlgExporter() {
}

void DlgExporter::initList() {
    GlobalImExportManager *ieman = GlobalImExportManager::instance();
    ui.cbPluginName->addItem(QString("< No Exporter Selected >"));
    int count = ieman->exporterCount();
    for (int i = 0; i < count; ++i) {
        anno::ExporterPlugin *p = ieman->getExporter(i);
        if (p != NULL) {
            ui.cbPluginName->addItem(p->name());
        }
    }
}

void DlgExporter::on_cbPluginName_currentIndexChanged(int index) {
    if (index >= 1) {
        anno::ExporterPlugin *p = GlobalImExportManager::instance()->getExporter(index - 1);
        if (p != NULL) {
            ui.editAuthor->setText(p->author());
            ui.txtDesc->setText(p->description());
        }
    }
}

int DlgExporter::getSelectedExporter() const {
    return ui.cbPluginName->currentIndex() - 1;
}

void DlgExporter::setSelectedExporter(int index) {
    if (index >= 0 && index < ui.cbPluginName->count()) {
        ui.cbPluginName->setCurrentIndex(index);
    }
}

anno::ExporterPlugin *DlgExporter::getSelectedExporterPtr() const {
    anno::ExporterPlugin *p = GlobalImExportManager::instance()->getExporter(getSelectedExporter());
    return p;
}

void DlgExporter::accept() {
    if (ui.cbPluginName->currentIndex() <= 0) {
        QMessageBox::critical( this, "AnnoTool", "Please select an exporter plugin.");
    } else {
        QDialog::accept();
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
