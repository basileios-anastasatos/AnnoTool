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
        }

        AnnoBoundingBox::~AnnoBoundingBox() {
            if (NULL != _segmImage) {
                delete _segmImage;
            }
            _segmImage = NULL;
        }

        AnnoShapeType AnnoBoundingBox::shapeType() const {
            return ASTypeBoundingBox;
        }

        QString AnnoBoundingBox::shapeInfo() const {
            QString res = QString("Type: %1\nx: %2\ny: %3\nwidth: %4\nheight: %5\n").arg("BoundingBox").arg(x(), 0, 'f', 2).arg(y(), 0, 'f', 2).arg(width(), 0, 'f', 2).arg(height(), 0, 'f', 2);
            res += QString("image path:%1\nx: %2\ny: %3\nwidth: %4\nheight: %5").arg(_segmImagePath).arg(_segmRealBoundRect.x()).arg(_segmRealBoundRect.y()).arg(_segmRealBoundRect.width()).arg(_segmRealBoundRect.height());
            return res;

            //return QString("width: %1, height: %2\n x: %3, y: %4").arg(width(), 0, 'f', 2).arg(height(), 0, 'f', 2).arg(x(), 0, 'f', 2).arg(y(), 0, 'f', 2);
        }

        QRectF AnnoBoundingBox::boundingRect() const {
            return *((QRectF *)this);
        }

        void AnnoBoundingBox::print() const {
            QTextStream out(stdout);
            out << "[BoundingBox, (" << x() << "," << y() << "), (" << width() << "," << height() << ")]"
                << endl;
        }

        AnnoBoundingBox &AnnoBoundingBox::operator=(const QRectF &r) {
            setRect(r.x(), r.y(), r.width(), r.height());
            return *this;
        }

        AnnoBoundingBox &AnnoBoundingBox::operator=(const AnnoBoundingBox &r) {
            setRect(r.x(), r.y(), r.width(), r.height());
            return *this;
        }

        void AnnoBoundingBox::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("boundingBox");
            XmlHelper::writeXmlPoint(writer, x(), y());
            XmlHelper::writeXmlSize(writer, width(), height());
            writer.writeTextElement("imgPath", _segmImagePath);
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

            if (!XmlHelper::skipToStartElement("imgPath", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "imgPath", reader.name().toString());
            }
            _segmImagePath = reader.readElementText();
            _segmImage = new QImage(_segmImagePath);

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

        void AnnoBoundingBox::setImagePath(const QString &sPath) {
            if (_segmImagePath != sPath) {
                _segmImagePath = sPath;
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
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
