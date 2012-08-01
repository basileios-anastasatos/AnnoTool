#include "include/GrabCut.h"

#include <highgui.h>
#include <iostream>

using namespace std;

#define INFLATE 0.5

namespace util {
    InteractiveGrabcut::InteractiveGrabcut() :
        _filePath(""),
        _src(), _mask(), _fgd(), _bgd(), _resultWithMask(),
        _fg_color(0, 255, 0), _bg_color(255, 0, 0),
        _bNew(true), _bFGMaskNotEmpty(false), _bBGMaskNotEmpty(false) {
    }

    void InteractiveGrabcut::buildGrabCut(const QString &sPath, const QRectF &boundBoxAbsRect, const QImage *qSegmMask /*= NULL*/) {
        _filePath = (sPath.toUtf8().constData());
        cv::Mat inputWholeImg = cv::imread(_filePath);

        _rcAbsRect = cv::Rect(boundBoxAbsRect.x(), boundBoxAbsRect.y(), boundBoxAbsRect.width(), boundBoxAbsRect.height());

        int dXInflate = INFLATE * _rcAbsRect.width, dYInflate = INFLATE * _rcAbsRect.height;

        _rcAbsInflatedRect = cv::Rect( _rcAbsRect.x - dXInflate, _rcAbsRect.y - dYInflate, _rcAbsRect.width + 2 * dXInflate, _rcAbsRect.height + 2 * dYInflate);
        InteractiveGrabcut::extractImage(inputWholeImg, _rcAbsInflatedRect, _src);
        // _rcAbsInflatedRect could be changed in extractImage method, now it contains the actual extracted rectangle

        _mask = cv::Mat::ones(_src.size(), CV_8U) * cv::GC_PR_BGD;

        _rcRelBoundRect = cv::Rect(boundBoxAbsRect.x() - _rcAbsInflatedRect.x, boundBoxAbsRect.y() - _rcAbsInflatedRect.y, boundBoxAbsRect.width(), boundBoxAbsRect.height());

        _bNew = true;
        _bFGMaskNotEmpty = false;
        _bBGMaskNotEmpty = false;

        if (NULL == qSegmMask) { // created new segmentation
            return;
        }

        // opened existing segmentation
        _bNew = false;
        extractImage(inputWholeImg, _rcAbsRect, _resultWithMask);// todo  reuse later?

        cv::Mat segmMaskImg = qImage2Mat(*qSegmMask);

        const float alphaFG = 0.5f;
        const float alphaPFG = 0.8f;

        for(int y = 0; y < segmMaskImg.rows; ++y) {
            for(int x = 0; x < segmMaskImg.cols; ++x) {
                if(255 == segmMaskImg.at<cv::Vec3b>(y, x)[1]) {
                    cv::Vec3b &pix = _resultWithMask.at<cv::Vec3b>(y, x);
                    pix[0] = (uchar)(pix[0] * alphaFG + _fg_color[0] * (1 - alphaFG));
                    pix[1] = (uchar)(pix[1] * alphaFG + _fg_color[1] * (1 - alphaFG));
                    pix[2] = (uchar)(pix[2] * alphaFG + _fg_color[2] * (1 - alphaFG));

                    _mask.at<uchar>(y + _rcRelBoundRect.y, x + _rcRelBoundRect.x) = cv::GC_FGD;
                    if (!_bFGMaskNotEmpty) {
                        _bFGMaskNotEmpty = true;
                    }
                } else if(127 == segmMaskImg.at<cv::Vec3b>(y, x)[1]) {
                    cv::Vec3b &pix = _resultWithMask.at<cv::Vec3b>(y, x);
                    pix[0] = (uchar)(pix[0] * alphaPFG + _fg_color[0] * (1 - alphaPFG));
                    pix[1] = (uchar)(pix[1] * alphaPFG + _fg_color[1] * (1 - alphaPFG));
                    pix[2] = (uchar)(pix[2] * alphaPFG + _fg_color[2] * (1 - alphaPFG));

                    _mask.at<uchar>(y + _rcRelBoundRect.y, x + _rcRelBoundRect.x) = cv::GC_PR_FGD;
                    if (!_bFGMaskNotEmpty) {
                        _bFGMaskNotEmpty = true;
                    }
                } else if(255 == segmMaskImg.at<cv::Vec3b>(y, x)[0]) {
                    _mask.at<uchar>(y + _rcRelBoundRect.y, x + _rcRelBoundRect.x) = cv::GC_BGD;
                    if (!_bBGMaskNotEmpty) {
                        _bBGMaskNotEmpty = true;
                    }
                } else if(127 == segmMaskImg.at<cv::Vec3b>(y, x)[0]) {
                    _mask.at<uchar>(y + _rcRelBoundRect.y, x + _rcRelBoundRect.x) = cv::GC_PR_BGD;
                    if (!_bBGMaskNotEmpty) {
                        _bBGMaskNotEmpty = true;
                    }
                }
            }
        }
        for(int y = 0; y < _mask.rows; y++) {
            for(int x = 0; x < _mask.cols; x++) {
                if (y >= _rcRelBoundRect.y && y < (_rcRelBoundRect.y + _rcRelBoundRect.height) && x >= _rcRelBoundRect.x && x < (_rcRelBoundRect.x + _rcRelBoundRect.width)) {
                    continue;
                } else {
                    _mask.at<uchar>(y, x) = cv::GC_BGD;
                    if(!_bBGMaskNotEmpty) {
                        _bBGMaskNotEmpty = true;
                    }
                }
            }
        }
    }

    void InteractiveGrabcut::rebuildGrabCut(InteractiveGrabcut *newGrabCut) {
        QRectF oldRect(_rcAbsInflatedRect.x, _rcAbsInflatedRect.y, _rcAbsInflatedRect.width, _rcAbsInflatedRect.height);
        QRectF newRect(newGrabCut->_rcAbsInflatedRect.x, newGrabCut->_rcAbsInflatedRect.y, newGrabCut->_rcAbsInflatedRect.width, newGrabCut->_rcAbsInflatedRect.height);
        QRectF intersection = oldRect & newRect;

        _src.release();	// size = oldRect
        _src = newGrabCut->_src.clone(); // size = newRect

        cv::Mat maskOld = _mask.clone(); // size = oldRect
        _mask.release();
        //_mask = newGrabCut->_mask.clone(); // size = newRect
        _mask = cv::Mat::ones(_src.size(), CV_8U) * cv::GC_PR_BGD;

        _fgd.release();
        _bgd.release();
        _resultWithMask.release();// todo  create/copy ?

        _bNew = true;
        _bFGMaskNotEmpty = false;
        _bBGMaskNotEmpty = false;

        int xI = intersection.x();
        int yI = intersection.y();

        for(int y = yI; y < yI + intersection.height(); ++y) {
            for(int x = xI; x < xI + intersection.width(); ++x) {
                //relative coordinates
                int xNew = x - newRect.x();
                int yNew = y - newRect.y();
                int xOld = x - oldRect.x();
                int yOld = y - oldRect.y();

                uchar val = maskOld.at<uchar>(yOld, xOld);
                _mask.at<uchar>(yNew, xNew) = val;
                if (cv::GC_FGD == val) {
                    if(!_bFGMaskNotEmpty) {
                        _bFGMaskNotEmpty = true;
                    }
                }
                if (cv::GC_BGD == val) {
                    if(!_bBGMaskNotEmpty) {
                        _bBGMaskNotEmpty = true;
                    }
                }
            }
        }

        if(_bFGMaskNotEmpty && _bBGMaskNotEmpty) {
            _bNew = false;
        }

        _rcAbsRect = newGrabCut->_rcAbsRect;
        _rcAbsInflatedRect = newGrabCut->_rcAbsInflatedRect;
        _rcRelBoundRect = newGrabCut->_rcRelBoundRect;
    }

    void InteractiveGrabcut::recalculate(InteractiveGrabcut *curGrabCut, const QRectF &newBoundBoxRect) {
        QRectF oldRect = QRectF(curGrabCut->_rcAbsRect.x, curGrabCut->_rcAbsRect.y, curGrabCut->_rcAbsRect.width, curGrabCut->_rcAbsRect.height);
        if (!oldRect.intersects(newBoundBoxRect)) {	// start new GrabCut
            curGrabCut->resetAll();
            curGrabCut->buildGrabCut(QString::fromUtf8(curGrabCut->getImagePath().c_str()), newBoundBoxRect);
            return;
        }
        // rebuild GrabcCut
        InteractiveGrabcut *newGrabCut = new InteractiveGrabcut();
        newGrabCut->buildGrabCut(QString::fromUtf8(curGrabCut->getImagePath().c_str()), newBoundBoxRect);
        curGrabCut->rebuildGrabCut(newGrabCut);
        delete newGrabCut;
    }

    QImage *InteractiveGrabcut::getImageWithMask() {
        return mat2QImage(_resultWithMask);
    }

    std::string InteractiveGrabcut::getImagePath() {
        return _filePath;
    }

    void InteractiveGrabcut::updateMask(const QPainterPath &fgPath, const QPainterPath &bgPath) {
        if (fgPath.isEmpty() && bgPath.isEmpty()) {
            return;
        }

        int xOffset = _rcAbsInflatedRect.x;
        int yOffset = _rcAbsInflatedRect.y;

        for(int y = 0; y < _mask.rows; y++) {
            for(int x = 0; x < _mask.cols; x++) {
                QPointF pt(x + xOffset, y + yOffset);
                if(fgPath.contains(pt)) {
                    cv::circle(_mask, cv::Point(x, y), 2, cv::GC_FGD, -1);
                    //_mask.at<uchar>(y, x) = cv::GC_FGD;
                    if (!_bFGMaskNotEmpty) {
                        _bFGMaskNotEmpty = true;
                    }
                }
                if(bgPath.contains(pt)) {
                    cv::circle(_mask, cv::Point(x, y), 2, cv::GC_BGD, -1);
                    //_mask.at<uchar>(y, x) = cv::GC_BGD;
                    if (!_bBGMaskNotEmpty) {
                        _bBGMaskNotEmpty = true;
                    }
                }
            }
        }
    }

    cv::Mat InteractiveGrabcut::getFGImage(cv::Rect &rcRealBoundRect, cv::Mat &binMaskRes) {
        cv::Mat scribbled_src = _src.clone();
        binMaskRes = _src.clone();
        const float alphaFG = 0.5f;
        const float alphaPFG = 0.8f;
        int minX = _mask.cols + 1, maxX = -1, minY = _mask.rows + 1, maxY = -1;
        for(int y = 0; y < _mask.rows; y++) {
            for(int x = 0; x < _mask.cols; x++) {
                if(_mask.at<uchar>(y, x) == cv::GC_FGD) {
                    if (minX > x) {
                        minX = x;
                    }
                    if (minY > y) {
                        minY = y;
                    }
                    if (maxX < x) {
                        maxX = x;
                    }
                    if (maxY < y) {
                        maxY = y;
                    }

                    cv::Vec3b &pix = scribbled_src.at<cv::Vec3b>(y, x);
                    pix[0] = (uchar)(pix[0] * alphaFG + _fg_color[0] * (1 - alphaFG));
                    pix[1] = (uchar)(pix[1] * alphaFG + _fg_color[1] * (1 - alphaFG));
                    pix[2] = (uchar)(pix[2] * alphaFG + _fg_color[2] * (1 - alphaFG));

                    cv::Vec3b &pix1 = binMaskRes.at<cv::Vec3b>(y, x);
                    pix1[0] = 0;
                    pix1[1] = 255;
                    pix1[2] = 0;
                } else if(_mask.at<uchar>(y, x) == cv::GC_PR_FGD) {
                    if (minX > x) {
                        minX = x;
                    }
                    if (minY > y) {
                        minY = y;
                    }
                    if (maxX < x) {
                        maxX = x;
                    }
                    if (maxY < y) {
                        maxY = y;
                    }

                    cv::Vec3b &pix = scribbled_src.at<cv::Vec3b>(y, x);
                    pix[0] = (uchar)(pix[0] * alphaPFG + _fg_color[0] * (1 - alphaPFG));
                    pix[1] = (uchar)(pix[1] * alphaPFG + _fg_color[1] * (1 - alphaPFG));
                    pix[2] = (uchar)(pix[2] * alphaPFG + _fg_color[2] * (1 - alphaPFG));

                    cv::Vec3b &pix1 = binMaskRes.at<cv::Vec3b>(y, x);
                    pix1[0] = 0;
                    pix1[1] = 127;
                    pix1[2] = 0;
                } else if(_mask.at<uchar>(y, x) == cv::GC_BGD) {
                    cv::Vec3b &pix = binMaskRes.at<cv::Vec3b>(y, x);
                    pix[0] = 255;
                    pix[1] = 0;
                    pix[2] = 0;
                } else if(_mask.at<uchar>(y, x) == cv::GC_PR_BGD) {
                    cv::Vec3b &pix = binMaskRes.at<cv::Vec3b>(y, x);
                    pix[0] = 127;
                    pix[1] = 0;
                    pix[2] = 0;
                }
            }
        }
        rcRealBoundRect.x = minX;
        rcRealBoundRect.y = minY;
        rcRealBoundRect.width = maxX - minX;
        rcRealBoundRect.height = maxY - minY;
        return scribbled_src;
    }

    void InteractiveGrabcut::resetAll() {
        _src.release();
        _fgd.release();
        _bgd.release();
        _mask.release();
        _resultWithMask.release();
    }

    QImage *InteractiveGrabcut::execute(bool bUpdMask, QRect &realRect, QImage &qImgMaskRes) {
        std::cout << "start grabCut" << std::endl;

        if (_bNew) { // new segmentation
            if (!bUpdMask) {
                cv::grabCut(_src, _mask, _rcRelBoundRect, _bgd, _fgd, 2, cv::GC_INIT_WITH_RECT);
            } else { // we have brush path
//	        	cv::Mat tmp(src.size(),CV_8UC3, cv::Scalar(0,0,0));
//	        	cv::grabCut(src, tmp, m_rcBoundRect, bgd, fgd, 1, cv::GC_INIT_WITH_RECT);
//	        	mask &= tmp;
                for(int y = 0; y < _mask.rows; y++) {
                    for(int x = 0; x < _mask.cols; x++) {
                        if (y >= _rcRelBoundRect.y && y < (_rcRelBoundRect.y + _rcRelBoundRect.height) && x >= _rcRelBoundRect.x && x < (_rcRelBoundRect.x + _rcRelBoundRect.width)) {
                            continue;
                        } else {
                            _mask.at<uchar>(y, x) = cv::GC_BGD;
                            if(!_bBGMaskNotEmpty) {
                                _bBGMaskNotEmpty = true;
                            }
                        }
                    }
                }
                if(_bFGMaskNotEmpty && _bBGMaskNotEmpty) {
                    cv::grabCut(_src, _mask, cv::Rect(), _bgd, _fgd, 2, cv::GC_INIT_WITH_MASK);
                } else {
                    cv::grabCut(_src, _mask, _rcRelBoundRect, _bgd, _fgd, 2, cv::GC_INIT_WITH_RECT);
                }
            }
            _bNew = false;
        } else { // already existing segmentation
            if(bUpdMask && _bFGMaskNotEmpty && _bBGMaskNotEmpty) {
                cv::grabCut(_src, _mask, cv::Rect(), _bgd, _fgd, 2, cv::GC_INIT_WITH_MASK);
            } else {
                cv::grabCut(_src, _mask, _rcRelBoundRect, _bgd, _fgd, 2, cv::GC_EVAL);
            }
        }

        std::cout << "end grabCut" << std::endl;

        cv::Rect rcRealRect;
        cv::Mat imgMaskResult;	//image mask for saving
        cv::Mat resultImg = getFGImage(rcRealRect, imgMaskResult);

        // convert rcRealRect to global coordinates
        int xOffset = _rcAbsInflatedRect.x;
        int yOffset = _rcAbsInflatedRect.y;
        realRect = QRect(xOffset + rcRealRect.x, yOffset + rcRealRect.y, rcRealRect.width, rcRealRect.height);

        cv::Mat imgMaskExtracted;
        extractImage(imgMaskResult, _rcRelBoundRect, imgMaskExtracted);
        qImgMaskRes = *(mat2QImage(imgMaskExtracted));

        cv::Mat resultExtracted;
        extractImage(resultImg, _rcRelBoundRect, resultExtracted);
        std::cout << "end other stuff" << std::endl;
        return mat2QImage(resultExtracted);
    }

    cv::Mat InteractiveGrabcut::qImage2Mat(const QImage &qimage) {
        cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar *)qimage.bits(), qimage.bytesPerLine());
        cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
        int from_to[] = { 0, 0,  1, 1,  2, 2 };
        cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
        return mat2;
    }

    QImage *InteractiveGrabcut::mat2QImage(const cv::Mat3b &src) {
        QImage *dest = new QImage(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y) {
            const cv::Vec3b *srcrow = src[y];
            QRgb *destrow = (QRgb *)dest->scanLine(y);
            for (int x = 0; x < src.cols; ++x) {
                destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
            }
        }
        return dest;
    }

    void InteractiveGrabcut::extractImage(const cv::Mat &src, cv::Rect &rect, cv::Mat &res) {
        double dYStart, dYFinish;
        double dXStart, dXFinish;
        dYStart = (rect.y >= 0) ? rect.y : 0;
        dYFinish = (rect.y + rect.height) <= src.rows ? (rect.y + rect.height) : src.rows;
        dXStart = (rect.x >= 0) ? rect.x : 0;
        dXFinish = (rect.x + rect.width) <= src.cols ? (rect.x + rect.width) : src.cols;

        rect.x = dXStart;
        rect.y = dYStart;
        rect.width = dXFinish - dXStart;
        rect.height = dYFinish - dYStart;

        cv::Mat tmp = src.rowRange(dYStart, dYFinish);
        res = tmp.colRange(dXStart, dXFinish);
    }

//	    cv::Mat getBinMask()
//	    {
//	        cv::Mat binmask(mask.size(), CV_8U);
//	        binmask = mask & 1;
//	        return binmask;
//	    };
//
//	    cv::Mat getFG()
//	    {
//	        cv::Mat fg = cv::Mat::zeros(src.size(), src.type());
//	        src.copyTo(fg, getBinMask());
//	        return fg;
//	    };

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
