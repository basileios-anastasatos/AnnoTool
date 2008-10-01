#ifndef ANNOOPERATIONHELPER_H_
#define ANNOOPERATIONHELPER_H_

#include "annoTypesForward.h"
#include "AllAnnoExceptions.h"
#include <QUuid>
#include <QList>

namespace anno {

    namespace dt {

        class AnnoOperationHelper {
            private:
                AnnoFileData *_fileData;

            private:


            public:
                AnnoOperationHelper(AnnoFileData *annoFileData) throw(exc::NullPointerException *);

                // helper for child querying
            public:
                QList<Annotation *> getChildren(const QList<QUuid> &children);
                QList<Annotation *> getChildren(const QUuid &parentUuid);
                QList<Annotation *> getChildren(int parentIndex);
                QList<Annotation *> getChildren(Annotation *parentAnno);

            public:
                bool deleteAnnotation(const QUuid &uuid, bool deep = true);
                bool deleteAnnotation(int index, bool deep = true);
                bool deleteAnnotation(Annotation *anno, bool deep = true);

                bool deleteChildren(const QUuid &uuid, bool deep = true);
                bool deleteChildren(int index, bool deep = true);
                bool deleteChildren(Annotation *anno, bool deep = true);
        };

    } // end: namespace dt
} // end: namespace anno

#endif /* ANNOOPERATIONHELPER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
