#include "include/AnnoPolygon.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "XmlHelper.h"

namespace anno {
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoPolygon::AnnoPolygon() {
        }

        AnnoPolygon::~AnnoPolygon() {
        }

        AnnoShapeType AnnoPolygon::shapeType() const {
            return ASTypePolygon;
        }

        QString AnnoPolygon::shapeInfo() const {
            return QString("Type: %1\n#-Points: %2").arg("Polygon").arg(size());
        }

        void AnnoPolygon::print() const {
            QTextStream out(stdout);
            out << "[Poly, (" << size() << ")]" << endl;
        }

        AnnoPolygon &AnnoPolygon::operator=(const QPolygonF &r) {
            clear();
            QVectorIterator<QPointF> i(r);
            while(i.hasNext()) {
                append(i.next());
            }
            return *this;
        }

        AnnoPolygon &AnnoPolygon::operator=(const AnnoPolygon &r) {
            clear();
            QVectorIterator<QPointF> i(r);
            while(i.hasNext()) {
                append(i.next());
            }
            return *this;
        }

        void AnnoPolygon::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement("polygon");
            QVectorIterator<QPointF> i(*this);
            while(i.hasNext()) {
                QPointF p = i.next();
                XmlHelper::writeXmlPoint(writer, p.x(), p.y());
            }
            writer.writeEndElement();
        }

        void AnnoPolygon::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
            if (!reader.isStartElement() || reader.name() != "polygon") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "polygon", reader.name().toString());
            }

            QString tagPoint("point");
            QString tagPoly("polygon");
            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagPoint) {
                    QPointF p = XmlHelper::readXmlPoint(reader);
                    append(p);
                } else if (reader.isEndElement() && reader.name() == tagPoly) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
