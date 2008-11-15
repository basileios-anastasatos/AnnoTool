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
        return GlobalProjectManager::instance()->fileCount();
    } else {
        return 0;
    }
}

int AnnoFileListModelAdapter::columnCount(const QModelIndex &parent) const {
    return 2;
}

QVariant AnnoFileListModelAdapter::data(const QModelIndex &index, int role) const {
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if (pm != NULL && pm->isValid() && index.isValid()) {
        if (index.row() >= 0 && index.row() < pm->fileCount() && (role == Qt::DisplayRole || role == Qt::ToolTipRole)) {
            anno::dt::AnnoFileData *cur = pm->getAnnoFile(index.row());
            if (index.column() == 0) {
                QString name(cur->imageInfo()->imagePath().fileName());
                if(cur->isModified()) {
                    name = "* " + name;
                }
                if(cur->imageInfo()->frame() != NOFRAME) {
                    name.append(QString("#%1").arg(cur->imageInfo()->frame()));
                }
                return name;
            } else if (index.column() == 1) {
                return cur->annoCount();
            }
        }
    }
    return QVariant();
}

QVariant AnnoFileListModelAdapter::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return QVariant("Image");
        } else if (section == 1) {
            return QVariant("#");
        }
    }
    return QVariant();
}

void AnnoFileListModelAdapter::update() {
    reset();
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
