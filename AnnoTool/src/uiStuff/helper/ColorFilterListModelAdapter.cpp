#include "include/ColorFilterListModelAdapter.h"
#include "AnnoFilterManager.h"
#include "ColorFilterEntry.h"


ColorFilterListModelAdapter::ColorFilterListModelAdapter(anno::filter::AnnoFilterManager *filterMan, QObject *parent) :
    QAbstractListModel(parent), _filterMan(filterMan) {
    connect(_filterMan, SIGNAL(coloringAdded(::anno::filter::ColorFilterEntry *, int)), this, SLOT(onFM_coloringAdded(::anno::filter::ColorFilterEntry *, int)));
    connect(_filterMan, SIGNAL(coloringRemoved(QString, int)), this, SLOT(onFM_coloringRemoved(QString, int)));
    connect(_filterMan, SIGNAL(coloringMoved(int, int)), this, SLOT(onFM_coloringMoved(int, int)));
}

void ColorFilterListModelAdapter::onFM_coloringAdded(::anno::filter::ColorFilterEntry *entry, int index) {
    update();
}

void ColorFilterListModelAdapter::onFM_coloringRemoved(QString name, int index) {
    update();
}

void ColorFilterListModelAdapter::onFM_coloringMoved(int oldIndex, int newIndex) {
    update();
}

int ColorFilterListModelAdapter::rowCount(const QModelIndex &parent) const {
    return _filterMan->colorCount();
}

QVariant ColorFilterListModelAdapter::data(const QModelIndex &index, int role) const {
    if (index.row() >= 0 && index.row() < rowCount() && (role == Qt::DisplayRole || role == Qt::ToolTipRole)) {
        return _filterMan->getColorRule(index.row())->getFilter()->getName();
    }
    return QVariant();
}

QVariant ColorFilterListModelAdapter::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return QVariant("Color Filter");
        }
    }
    return QVariant();

}

void ColorFilterListModelAdapter::update() {
    reset();
}




// vim:ts=4:sts=4:sw=4:tw=80:expandtab
