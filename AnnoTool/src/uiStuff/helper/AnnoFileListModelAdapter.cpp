#include "include/AnnoFileListModelAdapter.h"
#include "GlobalProjectManager.h"

using ::anno::GlobalProjectManager;

AnnoFileListModelAdapter::AnnoFileListModelAdapter(QObject *parent) :
    QAbstractTableModel(parent) {
}

AnnoFileListModelAdapter::~AnnoFileListModelAdapter() {
}

int AnnoFileListModelAdapter::rowCount(const QModelIndex &parent) const {
    if (GlobalProjectManager::instance()->isValid()) {
        return GlobalProjectManager::instance()->files()->size();
    } else {
        return 0;
    }
}

int AnnoFileListModelAdapter::columnCount(const QModelIndex &parent) const {
    return 2;
}

QVariant AnnoFileListModelAdapter::data(const QModelIndex &index, int role) const {
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if (pm->isValid() && index.isValid()) {
        if (index.row() >= 0 && index.row() < pm->files()->size() && (role == Qt::DisplayRole || role == Qt::ToolTipRole)) {
            if (index.column() == 0) {
                return pm->files()->at(index.row())->imageInfo()->imagePath().fileName();
            } else if (index.column() == 1) {
                return pm->files()->at(index.row())->imageUuidAsString();
            }
        }
    }
    return QVariant();
}

QVariant AnnoFileListModelAdapter::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return QVariant("Image File");
        } else if (section == 1) {
            return QVariant("UUID");
        }
    }
    return QVariant();
}

void AnnoFileListModelAdapter::update() {
    reset();
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
