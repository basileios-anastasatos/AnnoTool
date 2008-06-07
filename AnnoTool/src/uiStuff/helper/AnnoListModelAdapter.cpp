#include "include/AnnoListModelAdapter.h"

#include "GlobalProjectManager.h"

using ::anno::GlobalProjectManager;

AnnoListModelAdapter::AnnoListModelAdapter(QObject *parent) :
    QAbstractTableModel(parent) {
}

AnnoListModelAdapter::~AnnoListModelAdapter() {
}

int AnnoListModelAdapter::rowCount(const QModelIndex &parent) const {
    if (GlobalProjectManager::instance()->isValid()) {
        anno::dt::AnnoFileData *cur = GlobalProjectManager::instance()->selectedFile();
        if (cur != NULL) {
            return cur->annoCount();
        }
    }

    return 0;
}

int AnnoListModelAdapter::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant AnnoListModelAdapter::data(const QModelIndex &index, int role) const {
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if (pm->isValid() && index.isValid()) {
        anno::dt::AnnoFileData *cur = GlobalProjectManager::instance()->selectedFile();

        if (cur != NULL && index.row() >= 0 && index.row() < cur->annoCount() && (role == Qt::DisplayRole || role == Qt::ToolTipRole)) {
            if (index.column() == 0) {
                anno::dt::Annotation *curAnno = cur->getAnnotation(index.row());
                QString data = curAnno->annoIdAsString();
                if(curAnno->isModified()) {
                    data = "* " + data;
                }
                return data;
            }
        }
    }
    return QVariant();
}

QVariant AnnoListModelAdapter::headerData(int section, Qt::Orientation orientation,
        int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return QVariant("Annotation UUID");
        }
    }
    return QVariant();
}

void AnnoListModelAdapter::update() {
    reset();
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
