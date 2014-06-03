#include "include/AfrShape.h"
#include "Annotation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "XmlHelper.h"
#include "AnnoShape.h"

using anno::helper::XmlHelper;

namespace anno {
    namespace filter {
        const QString AfrShape::XML_NAME("hasShape");

        AfrShape::AfrShape() :
            AnnoFilterRuleAtom(true) {
        }

        AfrShape::AfrShape(const QString &shape, bool autoDelete) :
            AnnoFilterRuleAtom(autoDelete), _shape(shape) {
        }

        AfrShape::~AfrShape() {
        }

        AfrShape *AfrShape::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            AfrShape *pRule = new AfrShape();

            try {
                pRule->loadFromXml(reader);
            } catch(exc::XmlException *e) {
                delete pRule;
                throw e;
            }

            return pRule;
        }

        bool AfrShape::isValid() const {
            return !_shape.isEmpty();
        }

        QString AfrShape::toString() const {
            return QString("<Has Shape [%1]>").arg(_shape);
        }

        void AfrShape::toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            writer.writeEmptyElement(XML_NAME);
            writer.writeAttribute("name", _shape);
        }

        void AfrShape::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            QString curParent = reader.name().toString();
            if(!reader.isStartElement() || !isXmlName(curParent)) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_NAME, curParent);
            }

            QString shape = reader.attributes().value("name").toString();

            if(shape.isEmpty()) {
                throw new exc::XmlFormatException(__FILE__, __LINE__, "Shape must not be empty!");
            }
            _shape = shape;
            XmlHelper::skipToEndElement(XML_NAME, reader);
            reader.readNext();
        }

        bool AfrShape::evalInternal(const dt::Annotation *anno) const throw(exc::IllegalStateException *) {
            if(_shape.isEmpty()) {
                throw new exc::IllegalStateException(__FILE__, __LINE__, "<hasShape>-rule has invalid config.");
            }
            return shapeMap.contains(_shape) &&
                   (shapeMap[_shape] == anno->shape()->shapeType());
        }

        QMap<QString, anno::dt::AnnoShapeType> AfrShape::shapeMap;
        const bool AfrShape::dummy = AfrShape::initShapeMap();
        bool AfrShape::initShapeMap() {
            shapeMap["single point"] = shapeMap["sp"] = dt::ASTypeSinglePoint;
            shapeMap["line"]         = shapeMap["ln"] = dt::ASTypeLine;
            shapeMap["rectangle"]    = shapeMap["rc"] = dt::ASTypeRectangle;
            shapeMap["polygon"]      = shapeMap["pg"] = dt::ASTypePolygon;
            shapeMap["ellipse"]      = shapeMap["el"] = dt::ASTypeEllipse;
            shapeMap["segmentation"] = shapeMap["sg"] = dt::ASTypeSegmentation;
            shapeMap["path"]         = shapeMap["pt"] = dt::ASTypePath;
            return true;
        }
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
