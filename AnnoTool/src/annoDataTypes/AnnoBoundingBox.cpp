#include "include/AnnoBoundingBox.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "XmlHelper.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

namespace anno {
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoBoundingBox::AnnoBoundingBox() {
            _segmImage = NULL;
            _segmMask = NULL;
            _segmMaskPath = "";
        }

        AnnoBoundingBox::~AnnoBoundingBox() {
            if (NULL != _segmImage) {
                delete _segmImage;
            }
            _segmImage = NULL;

            if (NULL != _segmMask) {
                delete _segmMask;
            }
            _segmMask = NULL;
        }

        AnnoShapeType AnnoBoundingBox::shapeType() const {
            return ASTypeBoundingBox;
        }

        QString AnnoBoundingBox::shapeInfo() const {
            QString res = QString("Type: %1\nx: %2\ny: %3\nwidth: %4\nheight: %5\n").arg("BoundingBox").arg((int)(x())).arg((int)(y())).arg((int)(width())).arg((int)(height()));
            res += QString("image path:%1\nx: %2\ny: %3\nwidth: %4\nheight: %5").arg(_segmMaskPath).arg(_segmRealBoundRect.x()).arg(_segmRealBoundRect.y()).arg(_segmRealBoundRect.width()).arg(_segmRealBoundRect.height());
            return res;
        }

        QRectF AnnoBoundingBox::boundingRect() const {
            QRectF tmp = *((QRectF *)this);
            return QRectF((int)(tmp.x()), (int)(tmp.y()), (int)(tmp.width()), (int)(tmp.height()));
        }

        void AnnoBoundingBox::print() const {
            QTextStream out(stdout);
            out << "[BoundingBox, (" << (int)(x()) << "," << (int)(y()) << "), (" << (int)(width()) << "," << (int)(height()) << ")]" << endl;
        }

        AnnoBoundingBox &AnnoBoundingBox::operator=(const QRectF &r) {
            setRect((int)(r.x()), (int)(r.y()), (int)(r.width()), (int)(r.height()));
            return *this;
        }

        AnnoBoundingBox &AnnoBoundingBox::operator=(const AnnoBoundingBox &r) {
            setRect((int)(r.x()), (int)(r.y()), (int)(r.width()), (int)(r.height()));
            return *this;
        }

        void AnnoBoundingBox::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("boundingBox");
            XmlHelper::writeXmlPoint(writer, (int)(x()), (int)(y()));
            XmlHelper::writeXmlSize(writer, (int)(width()), (int)(height()));
            writer.writeTextElement("maskPath", _segmMaskPath);
            XmlHelper::writeXmlPoint(writer, _segmRealBoundRect.x(), _segmRealBoundRect.y());
            XmlHelper::writeXmlSize(writer, _segmRealBoundRect.width(), _segmRealBoundRect.height());
            writer.writeEndElement();
        }

        void AnnoBoundingBox::loadFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != "boundingBox") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "boundingBox", reader.name().toString());
            }
            if (!XmlHelper::skipToStartElement("point", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "point", reader.name().toString());
            }
            setTopLeft(XmlHelper::readXmlPoint(reader));

            if (!XmlHelper::skipToStartElement("size", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "size", reader.name().toString());
            }
            setSize(XmlHelper::readXmlSize(reader));

            if (!XmlHelper::skipToStartElement("maskPath", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "maskPath", reader.name().toString());
            }
            _segmMaskPath = reader.readElementText();
            _segmMask = new QImage(_segmMaskPath);

            if (!XmlHelper::skipToStartElement("point", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "point", reader.name().toString());
            }
            QPointF pt = XmlHelper::readXmlPoint(reader);
            _segmRealBoundRect.setTopLeft(QPoint(pt.x(), pt.y()));

            if (!XmlHelper::skipToStartElement("size", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "size", reader.name().toString());
            }
            QSizeF sz = XmlHelper::readXmlSize(reader);
            _segmRealBoundRect.setSize(QSize(sz.width(), sz.height()));
        }

        void AnnoBoundingBox::setImage(const QImage *segmImg) {
            if (NULL != _segmImage) {
                delete _segmImage;
            }
            _segmImage = new QImage( (const_cast<QImage *>(segmImg))->copy() );
        }

        void AnnoBoundingBox::resetImage() {
            if (NULL != _segmImage) {
                delete _segmImage;
            }
            _segmImage = NULL;
        }

        void AnnoBoundingBox::setMask(const QImage *segmMask) {
            if (NULL != _segmMask) {
                delete _segmMask;
            }
            _segmMask = new QImage( (const_cast<QImage *>(segmMask))->copy() );
        }

        void AnnoBoundingBox::setMaskPath(const QString &sPath) {
            if (_segmMaskPath != sPath) {
                _segmMaskPath = sPath;
            }
        }

        void AnnoBoundingBox::setRealBoundRect(QRect &rect) {
            _segmRealBoundRect.setX(rect.x());
            _segmRealBoundRect.setY(rect.y());
            _segmRealBoundRect.setWidth(rect.width());
            _segmRealBoundRect.setHeight(rect.height());
        }

        QImage *AnnoBoundingBox::getImage() {
            return _segmImage;
        }

        QImage *AnnoBoundingBox::getMask() {
            return _segmMask;
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        cv::Mat qimage2mat(const QImage &qimage) {
            cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar *)qimage.bits(), qimage.bytesPerLine());
            cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
            int from_to[] = { 0, 0,  1, 1,  2, 2 };
            cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
            return mat2;
        }

        QImage *Mat2QImage(const cv::Mat3b &src) {
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

        void extractImage(const cv::Mat &src, cv::Rect &rect, cv::Mat &res) {
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void AnnoBoundingBox::buildImageByMask(const QString &sPath) {
            const std::string input_filename(sPath.toUtf8().constData());
            cv::Mat wholeImage = cv::imread(input_filename);

            cv::Rect rect( (int)(x()), (int)(y()), (int)(width()), (int)(height()) );
            cv::Mat extractedImg;
            extractImage(wholeImage, rect, extractedImg);

            cv::Mat mask = qimage2mat(*_segmMask);

            cv::Scalar fg_color(0, 0, 255);
            const float alpha = 0.7f;

            int nCount = 0;
            int nTotal = mask.rows * mask.cols;

            for(int y = 0; y < mask.rows; y++) {
                for(int x = 0; x < mask.cols; x++) {
                    if(255 == mask.at<cv::Vec3b>(y, x)[2]) {
                        cv::circle(extractedImg, cv::Point(x, y), 2, fg_color, -1);
                    } else if(127 == mask.at<cv::Vec3b>(y, x)[2]) {
                        ++nCount;
                        cv::Vec3b &pix = extractedImg.at<cv::Vec3b>(y, x);
                        pix[0] = (uchar)(pix[0] * alpha + fg_color[0] * (1 - alpha));
                        pix[1] = (uchar)(pix[1] * alpha + fg_color[1] * (1 - alpha));
                        pix[2] = (uchar)(pix[2] * alpha + fg_color[2] * (1 - alpha));
                    }
                }
            }

            setImage(Mat2QImage(extractedImg));
        }
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
