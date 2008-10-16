#ifndef ANNOFILELISTSORTER_H_
#define ANNOFILELISTSORTER_H_

#include <QList>
#include <QString>
#include "annoTypesForward.h"

namespace anno {
    namespace dt {

        class AnnoFileListSorter {
            private:
                QList<AnnoFileData *> *_fileList;
                static int _frameSpacing;

            private:
                static int countDigits(int number);
                static bool afdLessThan_noframe(const dt::AnnoFileData *fd1, const dt::AnnoFileData *fd2);
                static bool afdLessThan_frame(const dt::AnnoFileData *fd1, const dt::AnnoFileData *fd2);
                int getMaxFrame();
                void setupPathFormat();

            public:
                AnnoFileListSorter(QList<dt::AnnoFileData *> *list);

            public:
                void sort();
        };

    }
}

#endif /* ANNOFILELISTSORTER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
