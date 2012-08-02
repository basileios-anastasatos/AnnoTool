#include "include/AnnoSegmentation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "XmlHelper.h"

namespace anno {
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoSegmentation::AnnoSegmentation() {
            _segmImage = NULL;
            _segmMask = NULL;
            _segmMaskPath = "";
        }

        AnnoSegmentation::~AnnoSegmentation() {
            if (NULL != _segmImage) {
                delete _segmImage;
            }
            _segmImage = NULL;

            if (NULL != _segmMask) {
                delete _segmMask;
            }
            _segmMask = NULL;
        }

        AnnoShapeType AnnoSegmentation::shapeType() const {
            return ASTypeSegmentation;
        }

        QString AnnoSegmentation::shapeInfo() const {
            QString res = QString("Type: %1\nx: %2 y: %3\nwidth: %4 height: %5\n").arg("BoundingBox").arg((int)(x())).arg((int)(y())).arg((int)(width())).arg((int)(height()));
            //res += QString("image path:%1\nx: %2\ny: %3\nwidth: %4\nheight: %5").arg(_segmMaskPath).arg(_segmRealBoundRect.x()).arg(_segmRealBoundRect.y()).arg(_segmRealBoundRect.width()).arg(_segmRealBoundRect.height());
            res += QString("MinMax: x: %1 y: %2\nwidth: %3 height: %4").arg(_segmRealBoundRect.x()).arg(_segmRealBoundRect.y()).arg(_segmRealBoundRect.width()).arg(_segmRealBoundRect.height());
            return res;
        }

        QRectF AnnoSegmentation::boundingRect() const {
            QRectF tmp = *((QRectF *)this);
            return QRectF((int)(tmp.x()), (int)(tmp.y()), (int)(tmp.width()), (int)(tmp.height()));
            //return *((QRectF*)this);
        }

        void AnnoSegmentation::print() const {
            QTextStream out(stdout);
            out << "[BoundingBox, (" << (int)(x()) << "," << (int)(y()) << "), (" << (int)(width()) << "," << (int)(height()) << ")]" << endl;
        }

        AnnoSegmentation &AnnoSegmentation::operator=(const QRectF &r) {
            setRect(r.x(), r.y(), r.width(), r.height());
            return *this;
        }

        AnnoSegmentation &AnnoSegmentation::operator=(const AnnoSegmentation &r) {
            setRect(r.x(), r.y(), r.width(), r.height());
            return *this;
        }

        void AnnoSegmentation::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("segment");

            writer.writeEmptyElement("boundBox");
            writer.writeAttribute("x", QString::number(int(x())));
            writer.writeAttribute("y", QString::number(int(y())));
            writer.writeAttribute("w", QString::number(int(width())));
            writer.writeAttribute("h", QString::number(int(height())));

            writer.writeTextElement("maskPath", _segmMaskPath);

            writer.writeEmptyElement("minMaxBox");
            writer.writeAttribute("x", QString::number(_segmRealBoundRect.x()));
            writer.writeAttribute("y", QString::number(_segmRealBoundRect.y()));
            writer.writeAttribute("w", QString::number(_segmRealBoundRect.width()));
            writer.writeAttribute("h", QString::number(_segmRealBoundRect.height()));

            writer.writeEndElement();
        }

        void AnnoSegmentation::loadFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != "segment") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "segment", reader.name().toString());
            }

            // read boundBox
            if (!XmlHelper::skipToStartElement("boundBox", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "boundBox", reader.name().toString());
            }
            QPointF pt = XmlHelper::readXmlPoint(reader);
            QSizeF sz = XmlHelper::readXmlSize(reader);
            setTopLeft(pt);
            setSize(sz);

            // read maskPath
            if (!XmlHelper::skipToStartElement("maskPath", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "maskPath", reader.name().toString());
            }
            _segmMaskPath = reader.readElementText();

            // read mask image
            _segmMask = new QImage(_segmMaskPath);

            // read minMaxBox
            if (!XmlHelper::skipToStartElement("minMaxBox", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "minMaxBox", reader.name().toString());
            }
            pt = XmlHelper::readXmlPoint(reader);
            sz = XmlHelper::readXmlSize(reader);
            _segmRealBoundRect.setTopLeft(QPoint(pt.x(), pt.y()));
            _segmRealBoundRect.setSize(QSize(sz.width(), sz.height()));
        }

        void AnnoSegmentation::setImage(const QImage *segmImg) {
            if (NULL != _segmImage) {
                delete _segmImage;
            }
            _segmImage = new QImage( (const_cast<QImage *>(segmImg))->copy() );
        }

        void AnnoSegmentation::resetImage() {
            if (NULL != _segmImage) {
                delete _segmImage;
            }
            _segmImage = NULL;

            if (NULL != _segmMask) {
                delete _segmMask;
            }
            _segmMask = NULL;
        }

        void AnnoSegmentation::setMask(const QImage *segmMask) {
            if (NULL != _segmMask) {
                delete _segmMask;
            }
            _segmMask = new QImage( (const_cast<QImage *>(segmMask))->copy() );
        }

        void AnnoSegmentation::setMaskPath(const QString &sPath) {
            if (_segmMaskPath != sPath) {
                _segmMaskPath = sPath;
            }
        }

        void AnnoSegmentation::setRealBoundRect(QRect &rect) {
            _segmRealBoundRect.setX(rect.x());
            _segmRealBoundRect.setY(rect.y());
            _segmRealBoundRect.setWidth(rect.width());
            _segmRealBoundRect.setHeight(rect.height());
        }

        QImage *AnnoSegmentation::getImage() {
            return _segmImage;
        }

        QImage *AnnoSegmentation::getMask() {
            return _segmMask;
        }
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
