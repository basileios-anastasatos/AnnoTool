#include "include/AnnoBoundingBox.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "XmlHelper.h"

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
            //res += QString("image path:%1\nx: %2\ny: %3\nwidth: %4\nheight: %5").arg(_segmMaskPath).arg(_segmRealBoundRect.x()).arg(_segmRealBoundRect.y()).arg(_segmRealBoundRect.width()).arg(_segmRealBoundRect.height());
            res += QString("REAL: x: %1\ny: %2\nwidth: %3\nheight: %4").arg(_segmRealBoundRect.x()).arg(_segmRealBoundRect.y()).arg(_segmRealBoundRect.width()).arg(_segmRealBoundRect.height());
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

            if (NULL != _segmMask) {
                delete _segmMask;
            }
            _segmMask = NULL;
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
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
