#include "include/DlgIdlExporter.h"

DlgIdlExporter::DlgIdlExporter(QWidget *parent)
    : QDialog(parent) {
    ui.setupUi(this);
}

DlgIdlExporter::~DlgIdlExporter() {

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


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
