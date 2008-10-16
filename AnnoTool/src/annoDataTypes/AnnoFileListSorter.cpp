#include "include/AnnoFileListSorter.h"
#include "AnnoFileData.h"

namespace anno {
    namespace dt {

        int AnnoFileListSorter::_frameSpacing;

        AnnoFileListSorter::AnnoFileListSorter(QList<dt::AnnoFileData *> *list) : _fileList(list) {
            // nothing to do here.
        }

        int AnnoFileListSorter::countDigits(int number) {
            if(number < 0) {
                number = -number;
            }

            if (number != 0) {
                int n = 0;
                while (number > 0) {
                    ++n;
                    number /= 10;
                }
                return n;
            } else {
                return 1;
            }
        }

        bool AnnoFileListSorter::afdLessThan_noframe(const dt::AnnoFileData *fd1, const dt::AnnoFileData *fd2) {
            return (fd1->imageInfo()->imagePath().filePath() < fd2->imageInfo()->imagePath().filePath());
        }

        bool AnnoFileListSorter::afdLessThan_frame(const dt::AnnoFileData *fd1, const dt::AnnoFileData *fd2) {
            QString s1 = QString("%1#%2").arg(fd1->imageInfo()->imagePath().filePath());
            QString s2 = QString("%1#%2").arg(fd2->imageInfo()->imagePath().filePath());
            int f1 = fd1->imageInfo()->frame();
            int f2 = fd2->imageInfo()->frame();

            if(f1 == NOFRAME) {
                f1 = 0;
            }
            if(f2 == NOFRAME) {
                f2 = 0;
            }

            s1 = s1.arg(f1, _frameSpacing, 10, QChar('0'));
            s2 = s2.arg(f2, _frameSpacing, 10, QChar('0'));

            return (s1 < s2);
        }

        int AnnoFileListSorter::getMaxFrame() {
            int maxFrame = -1;
            int curFrame = -1;
            foreach(AnnoFileData * cur, *_fileList) {
                curFrame = cur->imageInfo()->frame();
                if (curFrame != NOFRAME && curFrame > maxFrame) {
                    maxFrame = curFrame;
                }
            }

            return maxFrame;
        }

        void AnnoFileListSorter::setupPathFormat() {
            _frameSpacing = countDigits(getMaxFrame()) + 1;
        }

        void AnnoFileListSorter::sort() {
            setupPathFormat();
            if(_frameSpacing == 0) {
                qSort(_fileList->begin(), _fileList->end(), afdLessThan_noframe);
            } else {
                qSort(_fileList->begin(), _fileList->end(), afdLessThan_frame);
            }
        }


    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
