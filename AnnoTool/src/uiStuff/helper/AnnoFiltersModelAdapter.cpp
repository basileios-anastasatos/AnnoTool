#include "include/AnnoFiltersModelAdapter.h"
#include "AnnoFilterManager.h"
#include "AnnoFilter.h"

AnnoFiltersModelAdapter::AnnoFiltersModelAdapter(anno::filter::AnnoFilterManager *filterMan, QObject *parent) :
    QAbstractListModel(parent), _filterMan(filterMan) {
    _filterList = filterMan->getAllFilters();
    connect(_filterMan, SIGNAL(filterAdded(::anno::filter::AnnoFilter *)), this, SLOT(onFM_filterAdded(::anno::filter::AnnoFilter *)));
    connect(_filterMan, SIGNAL(filterRemoved(QString)), this, SLOT(onFM_filterRemoved(QString)));
}

void AnnoFiltersModelAdapter::onFM_filterAdded(::anno::filter::AnnoFilter *filter) {
    _filterList = _filterMan->getAllFilters();
    update();
}

void AnnoFiltersModelAdapter::onFM_filterRemoved(QString filterName) {
    _filterList = _filterMan->getAllFilters();
    update();
}

int AnnoFiltersModelAdapter::rowCount(const QModelIndex &parent) const {
    return _filterList.size();
}

QVariant AnnoFiltersModelAdapter::data(const QModelIndex &index, int role) const {
    if (index.row() >= 0 && index.row() < _filterList.size() && (role == Qt::DisplayRole || role == Qt::ToolTipRole)) {
        return _filterList[index.row()]->getName();
    }
    return QVariant();
}

QVariant AnnoFiltersModelAdapter::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return QVariant("Filter Name");
        }
    }
    return QVariant();
}

void AnnoFiltersModelAdapter::update() {
    reset();
}




// vim:ts=4:sts=4:sw=4:tw=80:expandtab
