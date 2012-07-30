#include "include/AnnoShape.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "XmlHelper.h"
#include "AnnoRectangle.h"
#include "AnnoSinglePoint.h"
#include "AnnoEllipse.h"
#include "AnnoPolygon.h"
#include "AnnoBoundingBox.h"

//namespace AnnoTool
namespace anno {

    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoShape::AnnoShape() {
        }

        AnnoShape::~AnnoShape() {
        }

        AnnoShape *AnnoShape::fromXml(QXmlStreamReader &reader) throw(XmlException *) {
            QString tagShape("shape");
            if (!reader.isStartElement() || reader.name() != tagShape) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagShape, reader.name().toString());
            }

            AnnoShape *shape = NULL;
            XmlHelper::skipToNextStartElement(true, reader);
            if (reader.isStartElement() && reader.name() == "rect") {
                shape = new AnnoRectangle();
                shape->loadFromXml(reader);
            } else if (reader.isStartElement() && reader.name() == "singlePoint") {
                shape = new AnnoSinglePoint();
                shape->loadFromXml(reader);
            } else if (reader.isStartElement() && reader.name() == "ellipse") {
                shape = new AnnoEllipse();
                shape->loadFromXml(reader);
            } else if (reader.isStartElement() && reader.name() == "polygon") {
                shape = new AnnoPolygon();
                shape->loadFromXml(reader);
            } else if (reader.isStartElement() && reader.name() == "boundingBox") {
                shape = new AnnoBoundingBox();
                shape->loadFromXml(reader);
            } else {
                QString msg("Unsupported annotation shape: %1");
                msg.arg(reader.name().toString());
                throw new XmlException(__FILE__, __LINE__, msg);
            }

            XmlHelper::skipToEndElement(tagShape, reader);
            reader.readNext();

            return shape;
        }

        AnnoShape *AnnoShape::copyShape(const AnnoShape *shape) {
            if(shape->shapeType() == ASTypeSinglePoint) {
                AnnoSinglePoint *cs = new AnnoSinglePoint();
                *cs = *((AnnoSinglePoint *)shape);
                return cs;
            } else if(shape->shapeType() == ASTypeRectangle) {
                AnnoRectangle *cs = new AnnoRectangle();
                *cs = *((AnnoRectangle *)shape);
                return cs;
            } else if(shape->shapeType() == ASTypePolygon) {
                AnnoPolygon *cs = new AnnoPolygon();
                *cs = *((AnnoPolygon *)shape);
                return cs;
            } else if(shape->shapeType() == ASTypeEllipse) {
                AnnoEllipse *cs = new AnnoEllipse();
                *cs = *((AnnoEllipse *)shape);
                return cs;
            } else if(shape->shapeType() == ASTypeBoundingBox) {
                AnnoBoundingBox *cs = new AnnoBoundingBox();
                *cs = *((AnnoBoundingBox *)shape);
                return cs;
            }

            return NULL;
        }

    }//end namespace dt
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
