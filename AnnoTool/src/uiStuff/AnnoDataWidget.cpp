#include "include/AnnoDataWidget.h"

AnnoDataWidget::AnnoDataWidget(QWidget *parent)
    : QDockWidget(parent) {
    ui.setupUi(this);
    setWidget(ui.spacer);
}

AnnoDataWidget::~AnnoDataWidget() {

}

void AnnoDataWidget::annoSelectChanged(int row, QUuid anno) {

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
