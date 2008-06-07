#include "include/AnnoSinglePoint.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "XmlHelper.h"

namespace anno {
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoSinglePoint::AnnoSinglePoint() {
        }

        AnnoSinglePoint::~AnnoSinglePoint() {
        }

        AnnoShapeType AnnoSinglePoint::shapeType() const {
            return ASTypeSinglePoint;
        }

        void AnnoSinglePoint::print() const {
            QTextStream out(stdout);
            out << "[Point, (" << x() << "," << y() << ")]" << endl;
        }

        AnnoSinglePoint &AnnoSinglePoint::operator=(const QPointF &point) {
            setX(point.x());
            setY(point.y());
            return *this;
        }

        AnnoSinglePoint &AnnoSinglePoint::operator=(const AnnoSinglePoint &point) {
            setX(point.x());
            setY(point.y());
            return *this;
        }

        void AnnoSinglePoint::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("singlePoint");
            writer.writeAttribute("x", QString::number(x(), 'f', 2));
            writer.writeAttribute("y", QString::number(y(), 'f', 2));
            writer.writeEndElement();
        }

        void AnnoSinglePoint::loadFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != "singlePoint") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "singlePoint", reader.name().toString());
            }
            QString strX = reader.attributes().value("x").toString();
            QString strY = reader.attributes().value("y").toString();
            if (strX.isEmpty() || strY.isEmpty()) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid Xml-AnnoSinglePoint format.");
            }

            bool ok = false;
            double x = strX.toDouble(&ok);
            double y = strY.toDouble(&ok);
            if (!ok) {
                throw new XmlException(__FILE__, __LINE__, "Could not parse Xml-AnnoSinglePoint data.");
            }
            setX(x);
            setY(y);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
