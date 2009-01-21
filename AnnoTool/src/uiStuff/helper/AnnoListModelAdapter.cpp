#include "include/AnnoListModelAdapter.h"
#include "importGlobals.h"
#include "XmlHelper.h"


AnnoListModelAdapter::AnnoListModelAdapter(QObject *parent) :
    QAbstractItemModel(parent) {
}

AnnoListModelAdapter::~AnnoListModelAdapter() {
}

int AnnoListModelAdapter::parentScopeRow(anno::dt::AnnoFileData *curFile, const anno::dt::Annotation *anno) const {
    QUuid childUuid = anno->annoId();
    QUuid parentUuid = anno->annoParent();

    if(childUuid.isNull()) {
        return -1;
    }

    // no parent => child is toplevel
    if(parentUuid.isNull()) {
        QList<QUuid> children = curFile->annoParents();
        int row = children.indexOf(childUuid);
        return row;
    } else {
        anno::dt::Annotation *parentAnno = curFile->getAnnotation(parentUuid);
        if (parentAnno != NULL) {
            QList<QUuid> children = parentAnno->annoChildren();
            int row = children.indexOf(childUuid);
            return row;
        }
    }

    return -1;
}

bool AnnoListModelAdapter::hasChildren(const QModelIndex &parent) const {
    if (GlobalProjectManager::instance()->isValid()) {
        anno::dt::AnnoFileData *cur = GlobalProjectManager::instance()->selectedFile();
        if (cur != NULL) {
            if(!parent.isValid()) {
                return (cur->annoCount() != 0);
            } else {
                const anno::dt::Annotation *anno = cur->getAnnotation(parent.internalId());
                return (anno != NULL && anno->hasAnnoChildren());
            }
        }
    }

    return false;
}

int AnnoListModelAdapter::rowCount(const QModelIndex &parent) const {
    if (GlobalProjectManager::instance()->isValid()) {
        anno::dt::AnnoFileData *cur = GlobalProjectManager::instance()->selectedFile();
        if (cur != NULL) {
            // Virtual root item
            if(!parent.isValid()) {
                return cur->annoParentCount();
            }
            // child items
            else {
                qint64 id = parent.internalId();
                const anno::dt::Annotation *anno = cur->getAnnotation(id);
                if (anno != NULL) {
                    return anno->annoChildCount();
                }
            }
        }
    }

    return 0;
}

int AnnoListModelAdapter::columnCount(const QModelIndex &parent) const {
    return 2;
}

QVariant AnnoListModelAdapter::data(const QModelIndex &index, int role) const {
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if (pm->isValid() && index.isValid()) {
        anno::dt::AnnoFileData *cur = GlobalProjectManager::instance()->selectedFile();
        if (cur != NULL && index.row() >= 0 && (role == Qt::DisplayRole || role
                                                == Qt::ToolTipRole || role == Qt::DecorationRole)) {
            qint64 id = index.internalId();
            const anno::dt::Annotation *anno = cur->getAnnotation(id);
            if (anno != NULL && index.column() == 0) {
                if (role == Qt::DecorationRole) {
                    // Mark parent lock
                    if(anno->annoId() == GlobalToolManager::instance()->getLockedAnno()) {
                        return QColor::fromRgb(255, 0, 0);
                    } else {
                        if(anno->hasAnnoChildren()) {
                            return QColor::fromRgb(0, 0, 170);
                        } else {
                            return QColor::fromRgb(0, 190, 0);
                        }
                    }
                } else {
                    //QString data = anno->annoIdAsString();

                    QString data = anno::helper::XmlHelper::uuidAsString(anno->annoId());
                    if (anno->isModified()) {
                        data = "* " + data;
                    }
                    return data;
                }
            } else if(anno != NULL && index.column() == 1) {
                QString data;
                data = (anno->classCount() == 0) ? "<no class>"
                       : ((anno->classCount() == 1) ? anno->classes()[0]
                          : "<multiple classes>");
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
            return QVariant("UUID");
        } else if(section == 1) {
            return QVariant("Class");
        }
    }
    return QVariant();
}

Qt::ItemFlags AnnoListModelAdapter::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::ItemIsDropEnabled;
    }

    return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
}

Qt::DropActions AnnoListModelAdapter::supportedDropActions() const {
    return Qt::MoveAction;
}

QStringList AnnoListModelAdapter::mimeTypes () const {
    QStringList types;
    types << "application/annotool.anno";
    return types;
}

QMimeData *AnnoListModelAdapter::mimeData(const QModelIndexList &indexes) const {
    if(indexes.size() == 2) {
        QMimeData *mimeData = new QMimeData();
        QByteArray encodedData;
        QDataStream stream(&encodedData, QIODevice::WriteOnly);

        foreach (QModelIndex index, indexes) {
            if (index.isValid()) {
                stream << (qint32)index.internalId();
            }
        }

        mimeData->setData("application/annotool.anno", encodedData);
        return mimeData;
    }

    GlobalLogger::instance()->logDebug(QString("AnnoListModelAdapter::mimeData: invalid data: count=%1").arg(indexes.size()));
    return NULL;
}

bool AnnoListModelAdapter::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
    if (action == Qt::IgnoreAction) {
        return true;
    }
    if (!data->hasFormat("application/annotool.anno")) {
        return false;
    }
    if (column > 1) {
        return false;
    }

    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if(pm->isValid()) {
        anno::dt::AnnoFileData *curFile = pm->selectedFile();
        if(curFile != NULL) {
            QByteArray encodedData = data->data("application/annotool.anno");
            QDataStream stream(&encodedData, QIODevice::ReadOnly);
            QStringList newItems;
            qint32 annoIdx;
            stream >> annoIdx;

            anno::dt::Annotation *anno = curFile->getAnnotation(annoIdx);
            if(anno != NULL) {
                if(anno->hasAnnoParent()) {
                    anno::dt::Annotation *parentAnno = curFile->getAnnotation(anno->annoParent());
                    if(parentAnno != NULL) {
                        parentAnno->removeAnnoChild(anno->annoId());
                    }
                }

                // drop on top level
                if (!parent.isValid()) {
                    // append to top level
                    // note that the linear IDs must not be altered so the top-level order cannot be
                    // manipulated by the user!
                    anno->setAnnoParent(QUuid());
                    emit dropEventFinished();
                    return true;
                }
                // drop inside hierarchy
                else {
                    anno::dt::Annotation *parentAnno = curFile->getAnnotation(parent.internalId());
                    if (parentAnno != NULL) {
                        anno->setAnnoParent(parentAnno->annoId());
                        parentAnno->insertAnnoChild(row, anno->annoId());
                        emit dropEventFinished();
                        return true;
                    }
                }
            }
        }
    }

    GlobalLogger::instance()->logDebug("AnnoListModelAdapter::dropMimeData -> false (invalid drop)");
    return false;
}

QModelIndex AnnoListModelAdapter::index(int row, int column, const QModelIndex &parent) const {
//	if (hasIndex(row, column, parent))
//	{
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if (pm->isValid()) {
        anno::dt::AnnoFileData *cur = GlobalProjectManager::instance()->selectedFile();
        if (cur != NULL) {
            // Virtual root item
            if(!parent.isValid()) {
                QList<QUuid> parents = cur->annoParents();
                QUuid childUuid = parents.value(row);
                int childIdx = cur->getAnnotationIndex(childUuid);
//					GlobalLogger::instance()->logDebug(QString("INDEX (root): ChildIdx: [%1]  ChildUuid: [%2]").arg(childIdx).arg(childUuid));
                if(childIdx >= 0) {
                    return createIndex(row, column, childIdx);
                }
            } else {
                qint64 parentId = parent.internalId();
                const anno::dt::Annotation *parentAnno = cur->getAnnotation(parentId);
                if (parentAnno != NULL) {
                    QList<QUuid> children = parentAnno->annoChildren();
                    if (row >= 0 && row < children.size()) {
                        QUuid childUuid = children.value(row);
                        int childIndex = cur->getAnnotationIndex(childUuid);
//							GlobalLogger::instance()->logDebug(QString("INDEX: parentIdx: [%1]  ChildIdx: [%2]  ChildUuid: [%3]").arg(parentId).arg(childIndex).arg(childUuid));
                        if (childIndex >= 0) {
                            return createIndex(row, column, childIndex);
                        }
                    } else {
                        GlobalLogger::instance()->logDebug("AnnoListModelAdapter::index - invalid child row number.");
                    }
                } else {
                    GlobalLogger::instance()->logDebug("AnnoListModelAdapter::index - invalid parent.");
                }
            }
        } else {
            GlobalLogger::instance()->logDebug("AnnoListModelAdapter::index - no file selected.");
        }
    } else {
        GlobalLogger::instance()->logDebug("AnnoListModelAdapter::index - invalid project state.");
    }
//	}
//	else
//	{
//		GlobalLogger::instance()->logWarning("AnnoListModelAdapter::index - has no index.");
//	}

    return QModelIndex();
}

QModelIndex AnnoListModelAdapter::parent(const QModelIndex &index) const {
    // Virtual root item
    if(!index.isValid()) {
        return QModelIndex();
    }

    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if (pm->isValid()) {
        anno::dt::AnnoFileData *cur = GlobalProjectManager::instance()->selectedFile();
        if (cur != NULL) {
            qint64 childIdx = index.internalId();
            const anno::dt::Annotation *childAnno = cur->getAnnotation(childIdx);
            if (childAnno != NULL) {
                QUuid parentUuid = childAnno->annoParent();
                const anno::dt::Annotation *parentAnno = cur->getAnnotation(parentUuid);
                if (parentAnno != NULL) {
                    int row = parentScopeRow(cur, parentAnno);
                    if(row >= 0) {
                        int parentIdx = cur->getAnnotationIndex(parentAnno);
                        //GlobalLogger::instance()->logDebug(QString("AnnoListModelAdapter::parent - parent row [%1] - parent idx [%2].").arg(row).arg(parentIdx));
                        return createIndex(row, 0, parentIdx);
                    } else {
                        GlobalLogger::instance()->logDebug(QString("AnnoListModelAdapter::parent - invalid parent row number."));
                    }
                } else {
                    // there is no parent
                    return QModelIndex();
                }
            }
        }
    }

    GlobalLogger::instance()->logDebug("AnnoListModelAdapter::parent - Invalid call.");
    return QModelIndex();
}

void AnnoListModelAdapter::update() {
    reset();
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
