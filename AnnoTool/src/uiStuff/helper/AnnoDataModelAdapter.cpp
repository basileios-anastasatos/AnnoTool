#include "include/AnnoDataModelAdapter.h"
#include "importGlobals.h"

AnnoDataModelAdapter::AnnoDataModelAdapter(QObject *parent) :
    QAbstractTableModel(parent) {
}

AnnoDataModelAdapter::~AnnoDataModelAdapter() {
}

int AnnoDataModelAdapter::rowCount(const QModelIndex &parent) const {
    if (GlobalProjectManager::instance()->isValid() && GlobalProjectManager::instance()->selectedFile() != NULL) {
        anno::dt::Annotation *cur = GlobalProjectManager::instance()->selectedAnno();
        if (cur != NULL) {
            return cur->attributes()->size();
        }
    }

    return 0;
}

int AnnoDataModelAdapter::columnCount(const QModelIndex &parent) const {
    return 2;
}

QVariant AnnoDataModelAdapter::data(const QModelIndex &index, int role) const {
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if (pm->isValid() && index.isValid()) {
        anno::dt::Annotation *cur = GlobalProjectManager::instance()->selectedAnno();

        if (cur != NULL && index.row() >= 0 && index.row() < cur->attributes()->size() && (role == Qt::DisplayRole || role == Qt::ToolTipRole)) {
            if (index.column() == 0) {
                anno::dt::AnnoAttribute attr = cur->attributes()->at(index.row());
                if(attr.className.isEmpty()) {
                    return attr.name;
                } else {
                    return attr.className + ":" + attr.name;
                }
            } else if (index.column() == 1) {
                return cur->attributes()->at(index.row()).value;
            }
        }
    }
    return QVariant();
}

QVariant AnnoDataModelAdapter::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return QVariant("Name");
        } else if (section == 1) {
            return QVariant("Value");
        }
    }
    return QVariant();
}

void AnnoDataModelAdapter::update() {
    reset();
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
