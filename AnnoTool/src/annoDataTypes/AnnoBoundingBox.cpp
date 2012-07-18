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
        }

        AnnoShapeType AnnoBoundingBox::shapeType() const {
            return ASTypeBoundingBox;
        }

        QString AnnoBoundingBox::shapeInfo() const {
            //return QString("Type: %1\nx: %2\ny: %3\nwidth: %4\nheight: %5").arg("BoundingBox").arg(x(), 0, 'f', 2).arg(y(), 0, 'f', 2).arg(width(), 0, 'f', 2).arg(height(), 0, 'f', 2);

            return QString("width: %1, height: %2\n x: %3, y: %4").arg(width(), 0, 'f', 2).arg(height(), 0, 'f', 2).arg(x(), 0, 'f', 2).arg(y(), 0, 'f', 2);
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
            writer.writeEndElement();
        }

        void AnnoBoundingBox::loadFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != "rect") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "rect", reader.name().toString());
            }
            if (!XmlHelper::skipToStartElement("point", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "point", reader.name().toString());
            }
            setTopLeft(XmlHelper::readXmlPoint(reader));

            if (!XmlHelper::skipToStartElement("size", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "size", reader.name().toString());
            }
            setSize(XmlHelper::readXmlSize(reader));
        }

        void AnnoBoundingBox::setImage(const QImage *segmImg) {
            if (_segmImage != segmImg) {
                _segmImage = new QImage( (const_cast<QImage *>(segmImg))->copy() );
            }
        }

        QImage *AnnoBoundingBox::getImage() {
            return _segmImage;
        }
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
