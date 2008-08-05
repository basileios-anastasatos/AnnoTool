#include "include/AnnoEllipse.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "XmlHelper.h"
#include "importGlobals.h"

namespace anno {
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoEllipse::AnnoEllipse() {
        }

        AnnoEllipse::~AnnoEllipse() {
        }

        AnnoShapeType AnnoEllipse::shapeType() const {
            return ASTypeEllipse;
        }

        QString AnnoEllipse::shapeInfo() const {
            return QString("Type: %1\ncx: %2\ncy: %3\nradius-x: %4\nradius-y: %5").arg("Ellipse").arg(centerX(), 0, 'f', 2).arg(centerY(), 0, 'f', 2).arg(radiusX(), 0, 'f', 2).arg(radiusY(), 0, 'f', 2);
        }

        QRectF AnnoEllipse::boundingRect() const {
            return *((QRectF *)this);
        }

        void AnnoEllipse::print() const {
            QTextStream out(stdout);
            out << "[Ellipse, (" << centerX() << "," << centerY() << "), (" << radiusX() << ","
                << radiusY() << ")]" << endl;
        }

        void AnnoEllipse::setRadiusX(qreal r) {
            QPointF c = center();
            setWidth(2.0 * r);
            moveCenter(c);
        }

        void AnnoEllipse::setRadiusY(qreal r) {
            QPointF c = center();
            setHeight(2.0 * r);
            moveCenter(c);
        }

        void AnnoEllipse::setRadi(qreal rx, qreal ry) {
            QPointF c = center();
            setWidth(2.0 * rx);
            setHeight(2.0 * ry);
            moveCenter(c);
        }

        qreal AnnoEllipse::centerX() const {
            return (x() + (width() / 2.0));
        }

        qreal AnnoEllipse::centerY() const {
            return (y() + (height() / 2.0));
        }

        qreal AnnoEllipse::radiusX() const {
            return (width() / 2.0);
        }

        qreal AnnoEllipse::radiusY() const {
            return (height() / 2.0);
        }

        AnnoEllipse &AnnoEllipse::operator=(const QRectF &r) {
            setRect(r.x(), r.y(), r.width(), r.height());
            return *this;
        }

        AnnoEllipse &AnnoEllipse::operator=(const AnnoEllipse &r) {
            setRect(r.x(), r.y(), r.width(), r.height());
            return *this;
        }

        void AnnoEllipse::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement("ellipse");
            writer.writeAttribute("rx", QString::number(radiusX(), 'f', 2));
            writer.writeAttribute("ry", QString::number(radiusY(), 'f', 2));
            XmlHelper::writeXmlPoint(writer, "center", centerX(), centerY());
            writer.writeEndElement();
        }

        void AnnoEllipse::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != "ellipse") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "ellipse", reader.name().toString());
            }

            QString strRx = reader.attributes().value("rx").toString();
            QString strRy = reader.attributes().value("ry").toString();
            if (strRx.isEmpty() || strRy.isEmpty()) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid Xml-Ellipse format.");
            }

            bool ok = false;
            double rx = strRx.toDouble(&ok);
            double ry = strRy.toDouble(&ok);
            if (!ok) {
                throw new XmlException(__FILE__, __LINE__, "Could not parse Xml-Ellipse data.");
            }
            setRadi(rx, ry);

            if (!XmlHelper::skipToStartElement("center", reader)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "center", reader.name().toString());
            }
            moveCenter(XmlHelper::readXmlPoint(reader));

            GlobalLogger::instance()->logDebug("read XML-ELLIPSE");
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
