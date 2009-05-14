#include "include/AnnoOperationHelper.h"
#include "AllAnnoExceptions.h"
#include "AnnoFileData.h"
#include "Annotation.h"

namespace anno {
    namespace dt {

        AnnoOperationHelper::AnnoOperationHelper(AnnoFileData *annoFileData) throw(exc::NullPointerException *) :
            _fileData(NULL) {
            if (annoFileData != NULL) {
                _fileData = annoFileData;
            } else {
                throw new exc::NullPointerException(__FILE__, __LINE__,
                                                    "AnnoOperationHelper must not be instantiated with an invalid data pointer.");
            }
        }

        QList<Annotation *> AnnoOperationHelper::getChildren(const QList<QUuid> &children) {
            QList<Annotation *> childList;
            foreach(QUuid uuid, children) {
                Annotation *curAnno = _fileData->getAnnotation(uuid);
                if(curAnno != NULL) {
                    childList << curAnno;
                }
            }

            return childList;
        }

        QList<Annotation *> AnnoOperationHelper::getChildren(const QUuid &parentUuid) {
            Annotation *anno = _fileData->getAnnotation(parentUuid);
            return getChildren(anno);
        }

        QList<Annotation *> AnnoOperationHelper::getChildren(int parentIndex) {
            Annotation *anno = _fileData->getAnnotation(parentIndex);
            return getChildren(anno);
        }

        QList<Annotation *> AnnoOperationHelper::getChildren(Annotation *parentAnno) {
            if(parentAnno != NULL) {
                return getChildren(parentAnno->annoChildren());
            } else {
                return QList<Annotation *>();
            }
        }

        bool AnnoOperationHelper::deleteAnnotation(const QUuid &uuid, bool deep) {
            Annotation *anno = _fileData->getAnnotation(uuid);
            if(_fileData != NULL) {
                return deleteAnnotation(anno, deep);
            } else {
                return false;
            }
        }

        bool AnnoOperationHelper::deleteAnnotation(int index, bool deep) {
            Annotation *anno = _fileData->getAnnotation(index);
            if (_fileData != NULL) {
                return deleteAnnotation(anno, deep);
            } else {
                return false;
            }
        }

        bool AnnoOperationHelper::deleteAnnotation(Annotation *anno, bool deep) {
            if(anno == NULL || !_fileData->containsAnnotation(anno)) {
                return false;
            }

            QUuid parentUuid = anno->annoParent();
            Annotation *parentAnno = _fileData->getAnnotation(parentUuid);

            if(deep) {
                //delete all children and successors
                if(deleteChildren(anno, true)) {
                    if(parentAnno != NULL) {
                        parentAnno->removeAnnoChild(anno->annoId());
                    }
                    _fileData->removeAnnotation(anno->annoId());
                    return true;
                } else {
                    return false;
                }
            }
            // set all children's parent to annoParent
            else {
                QList<Annotation *> childList = getChildren(anno);
                int childIndex = 0;
                if(parentAnno != NULL) {
                    int tmp = parentAnno->annoChildren().indexOf(anno->annoId());
                    if(tmp >= 0) {
                        childIndex = tmp;
                    }
                }

                // update children's parent and parent's children
                foreach(Annotation * curChild, childList) {
                    curChild->setAnnoParent(parentUuid);
                    if(parentAnno != NULL) {
                        parentAnno->insertAnnoChild(childIndex++, curChild->annoId());
                    }
                }
                if(parentAnno != NULL) {
                    parentAnno->removeAnnoChild(anno->annoId());
                }
                _fileData->removeAnnotation(anno->annoId());
                return true;
            }
        }

        bool AnnoOperationHelper::deleteChildren(const QUuid &uuid, bool deep) {
            Annotation *anno = _fileData->getAnnotation(uuid);
            if (_fileData != NULL) {
                return deleteChildren(anno, deep);
            } else {
                return false;
            }
        }

        bool AnnoOperationHelper::deleteChildren(int index, bool deep) {
            Annotation *anno = _fileData->getAnnotation(index);
            if (_fileData != NULL) {
                return deleteChildren(anno, deep);
            } else {
                return false;
            }
        }

        bool AnnoOperationHelper::deleteChildren(Annotation *anno, bool deep) {
            if(anno == NULL) {
                return false;
            }

            // no children => nothing to do
            if(!anno->hasAnnoChildren()) {
                return true;
            }

            QList<Annotation *> childList = getChildren(anno);

            foreach(Annotation * curChild, childList) {

                if(deep) {
                    deleteChildren(curChild, deep);
                }

                _fileData->removeAnnotation(curChild->annoId());
            }
            return true;
        }

    } // end: namespace dt
} // end: namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
