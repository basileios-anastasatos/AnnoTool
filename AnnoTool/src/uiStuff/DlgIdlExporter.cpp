#include "include/DlgIdlExporter.h"
#include "DlgFilterEdit.h"
#include "importGlobals.h"

DlgIdlExporter::DlgIdlExporter(QWidget *parent)
    : QDialog(parent) {
    ui.setupUi(this);
    initFilters();
}

DlgIdlExporter::~DlgIdlExporter() {

}

void DlgIdlExporter::initFilters() {
    ui.cbFilters->clear();
    ui.cbFilters->addItem("< none >");

    if(GlobalProjectManager::instance()->filterMan() != NULL) {
        QList<anno::filter::AnnoFilter *> lst = GlobalProjectManager::instance()->filterMan()->getAllFilters();
        foreach(anno::filter::AnnoFilter * f, lst) {
            ui.cbFilters->addItem(f->getName());
        }
    }
}

void DlgIdlExporter::on_cbFilters_currentIndexChanged(int idx) {
    if(idx > 0 && GlobalProjectManager::instance()->filterMan() != NULL) {
        QList<anno::filter::AnnoFilter *> lst = GlobalProjectManager::instance()->filterMan()->getAllFilters();
        if((idx - 1) < lst.size()) {
            _selFilter = lst[idx - 1];
        } else {
            _selFilter = NULL;
        }
    } else {
        _selFilter = NULL;
    }
}

void DlgIdlExporter::on_btFilterEdit_clicked() {
    if (GlobalProjectManager::instance()->isValid()) {
        DlgFilterEdit dlg(GlobalProjectManager::instance()->filterMan());
        dlg.exec();
        initFilters();
    }
}

bool DlgIdlExporter::expRectangles() const {
    return ui.chkRectangle->isChecked();
}

bool DlgIdlExporter::expEllipses() const {
    return ui.chkEllipse->isChecked();
}

bool DlgIdlExporter::expPolygons() const {
    return ui.chkPolygon->isChecked();
}

bool DlgIdlExporter::expSinglePoints() const {
    return ui.chkSinglePoint->isChecked();
}

bool DlgIdlExporter::expAnything() const {
    return (ui.chkRectangle->isChecked() ||
            ui.chkEllipse->isChecked() ||
            ui.chkPolygon->isChecked() ||
            ui.chkSinglePoint->isChecked());
}

bool DlgIdlExporter::expAll() const {
    return (ui.chkRectangle->isChecked() &&
            ui.chkEllipse->isChecked() &&
            ui.chkPolygon->isChecked() &&
            ui.chkSinglePoint->isChecked());
}

bool DlgIdlExporter::pathsRelative() const {
    return ui.radioPathRelative->isChecked();
}
bool DlgIdlExporter::pathsAbsolute() const {
    return ui.radioPathAbsolute->isChecked();
}

bool DlgIdlExporter::hasFilter() const {
    return (_selFilter != NULL);
}

anno::filter::AnnoFilter *DlgIdlExporter::selectedFilter() const {
    return _selFilter;
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
