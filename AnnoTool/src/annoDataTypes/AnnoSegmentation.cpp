#include "include/AnnoSegmentation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "XmlHelper.h"

namespace anno {
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoSegmenation::AnnoSegmenation() {
            _segmImage = NULL;
        }

        AnnoSegmenation::~AnnoSegmenation() {
        }

        AnnoShapeType AnnoSegmenation::shapeType() const {
            return ASTypeSegmentation;
        }

        QString AnnoSegmenation::shapeInfo() const {
//			return QString("Type: %1\n#-Points: %2").arg("Segmentation").arg(size());
            return QString("Type: %1").arg("Segmentation");
        }

        QRectF AnnoSegmenation::boundingRect() const {
            return /*QImage::rect()*/_segmImage->rect();
        }

        void AnnoSegmenation::print() const {
            QTextStream out(stdout);
//			out <<"[Segmentation, (" <<size() <<")]" <<endl;
            out << "[Segmentation]" << endl;
        }

        AnnoSegmenation &AnnoSegmenation::operator=(const QPolygonF &r) {
//			clear();
//			QVectorIterator<QPointF> i(r);
//			while(i.hasNext())
//			{
//				append(i.next());
//			}
            return *this;
        }

        AnnoSegmenation &AnnoSegmenation::operator=(const AnnoSegmenation &r) {
//			clear();
//			QVectorIterator<QPointF> i(r);
//			while(i.hasNext())
//			{
//				append(i.next());
//			}
            return *this;
        }

        void AnnoSegmenation::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement("segmentation");
//			QVectorIterator<QPointF> i(*this);
//			while(i.hasNext())
//			{
//				QPointF p = i.next();
//				XmlHelper::writeXmlPoint(writer, p.x(), p.y());
//			}
//			writer.writeEndElement();
        }

        void AnnoSegmenation::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
//			if (!reader.isStartElement() || reader.name() != "segmentation")
            //		{
            //		throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "segmentation", reader.name().toString());
            //}

//			QString tagPoint("point");
//			QString tagPoly("polygon");
//			while (!reader.atEnd())
//			{
//				if (reader.isStartElement() && reader.name() == tagPoint)
//				{
//					QPointF p = XmlHelper::readXmlPoint(reader);
//					append(p);
//				}
//				else if (reader.isEndElement() && reader.name() == tagPoly)
//				{
//					reader.readNext();
//					break;
//				}
//				reader.readNext();
//			}
        }

        void AnnoSegmenation::setImage(const QImage *segmImg) {
            if (_segmImage != segmImg) {
                _segmImage = new QImage( (const_cast<QImage *>(segmImg))->copy() );
            }
        }

        QImage *AnnoSegmenation::getImage() {
            return _segmImage;
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
