#include "include/AnnoPath.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "XmlHelper.h"

namespace anno {
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoPath::AnnoPath() {
        }

        AnnoPath::~AnnoPath() {
        }

        AnnoShapeType AnnoPath::shapeType() const {
            return ASTypePath;
        }

        QString AnnoPath::shapeInfo() const {
            return QString("Type: %1\n").arg("Path");
        }

        QRectF AnnoPath::boundingRect() const {
            return QPainterPath::boundingRect();
        }

        QPainterPath AnnoPath::path() const {
            return *((QPainterPath *)this);
        }

        // todo
        void AnnoPath::print() const {
        }

        // todo
//		AnnoPath& AnnoPath::operator=(const QPainterPath& path)
//		{
//			return *this;
//		}
//
//		AnnoPath& AnnoPath::operator=(const AnnoPath& path)
//		{
//			return *this;
//		}

        void AnnoPath::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
        }

        void AnnoPath::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
