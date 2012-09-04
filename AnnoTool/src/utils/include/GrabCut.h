#pragma once

#include <QString>
#include <QImage>

#include <cv.h>
#include <cxcore.h>
#include <string>

namespace util {
    class InteractiveGrabcut {
            std::string _filePath;
            cv::Rect 	_rcAbsRect;
            cv::Rect 	_rcAbsInflatedRect;
            cv::Rect 	_rcRelBoundRect;

            cv::Mat 	_src;
            cv::Mat 	_fgd;
            cv::Mat 	_bgd;
            cv::Mat 	_mask;

            cv::Scalar 	_fg_color;
            cv::Scalar 	_bg_color;

            bool		_bNew;
            bool		_bFGMaskNotEmpty;
            bool		_bBGMaskNotEmpty;

        public:

            InteractiveGrabcut();

            void buildGrabCut(const QString &sPath, const QRectF &boundBoxRect, const QImage *qSegmMask = NULL);
            void rebuildGrabCut(InteractiveGrabcut *newGrabCut);
            static void recalculate(InteractiveGrabcut *curGrabCut, const QRectF &newBoundBoxRect);

            void cropSource(QRect &cropRect);
            void addFGPathToMask(const QPainterPath &fgPath);
            void addBGPathToMask(const QPainterPath &bgPath);
            QImage *execute(bool bUpdMask, QRect &realRect, QImage &qImgMaskRes);

            QImage *getImageWithMask();
            std::string getImagePath();

        protected:
            cv::Mat getFGImage(cv::Rect &rcRealBoundRect, cv::Mat &binMaskRes);
            void resetAll();

        public:
            static cv::Mat qImage2Mat(const QImage &qimage);
            static QImage *mat2QImage(const cv::Mat3b &src);
            static void extractImage(const cv::Mat &src, cv::Rect &rect, cv::Mat &res);
    };
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
