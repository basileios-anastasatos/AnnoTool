#include "include/AnnoDataClassesModelAdapter.h"
#include "importGlobals.h"

AnnoDataClassesModelAdapter::AnnoDataClassesModelAdapter(QObject *parent) :
    QAbstractTableModel(parent) {
}

AnnoDataClassesModelAdapter::~AnnoDataClassesModelAdapter() {
}

int AnnoDataClassesModelAdapter::rowCount(const QModelIndex &parent) const {
    if (GlobalProjectManager::instance()->isValid() && GlobalProjectManager::instance()->selectedAnno() != NULL) {
        anno::dt::Annotation *cur = GlobalProjectManager::instance()->selectedAnno();
        if (cur != NULL) {
            return cur->classes()->size();
        }
    }

    return 0;
}

int AnnoDataClassesModelAdapter::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant AnnoDataClassesModelAdapter::data(const QModelIndex &index, int role) const {
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if (pm->isValid() && index.isValid()) {
        anno::dt::Annotation *cur = GlobalProjectManager::instance()->selectedAnno();

        if (cur != NULL && index.row() >= 0 && index.row() < cur->classes()->size() && (role == Qt::DisplayRole || role == Qt::ToolTipRole)) {
            if (index.column() == 0) {
                QString cname = cur->classes()->at(index.row());
                return cname;
            }
        }
    }
    return QVariant();
}

QVariant AnnoDataClassesModelAdapter::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return QVariant("Class Name");
        }
    }
    return QVariant();
}

void AnnoDataClassesModelAdapter::update() {
    reset();
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
