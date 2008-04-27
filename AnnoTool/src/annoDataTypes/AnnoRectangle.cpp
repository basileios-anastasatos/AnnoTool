#include "include/AnnoRectangle.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "XmlHelper.h"

namespace anno {

    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoRectangle::AnnoRectangle() {
        }

        AnnoRectangle::~AnnoRectangle() {
        }

        AnnoShapeType AnnoRectangle::shapeType() const {
            return ASTypeRectangle;
        }

        void AnnoRectangle::print() const {
            QTextStream out(stdout);
            out << "[Rect, (" << x() << "," << y() << "), (" << width() << "," << height()
                << ")]" << endl;
        }

        AnnoRectangle &AnnoRectangle::operator=(const QRectF &r) {
            setRect(r.x(), r.y(), r.width(), r.height());
            return *this;
        }

        AnnoRectangle &AnnoRectangle::operator=(const AnnoRectangle &r) {
            setRect(r.x(), r.y(), r.width(), r.height());
            return *this;
        }

        void AnnoRectangle::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("rect");
            XmlHelper::writeXmlPoint(writer, x(), y());
            XmlHelper::writeXmlSize(writer, width(), height());
            writer.writeEndElement();
        }

        void AnnoRectangle::loadFromXml(QXmlStreamReader &reader)
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

    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
