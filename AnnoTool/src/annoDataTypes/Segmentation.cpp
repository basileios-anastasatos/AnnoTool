#include "include/Segmentation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "importGlobals.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        Segmentation::Segmentation(QObject *parent) {
            _grabCutContext = NULL;
        }

        Segmentation::Segmentation(const Segmentation *anno, QObject *parent) {
            _grabCutContext = NULL;
        }

        Segmentation::~Segmentation() {
            if (NULL != _grabCutContext) {
                delete _grabCutContext;
            }
            _grabCutContext = NULL;
        }

        void Segmentation::appendFGPath(const QPainterPath &fgPath) {
            _v_painterFGPath.push_back(fgPath);
        }

        void Segmentation::appendBGPath(const QPainterPath &bgPath) {
            _v_painterBGPath.push_back(bgPath);
        }

        void Segmentation::emptyFGPath() {
            _v_painterFGPath.clear();
        }

        void Segmentation::emptyBGPath() {
            _v_painterBGPath.clear();
        }

        const std::vector<QPainterPath> Segmentation::getFGPath() {
            return _v_painterFGPath;
        }

        const std::vector<QPainterPath> Segmentation::getBGPath() {
            return _v_painterBGPath;
        }

        util::InteractiveGrabcut *Segmentation::provideGrabCutContext() {
            if (NULL == _grabCutContext) {
                _grabCutContext = new util::InteractiveGrabcut();
            }
            return _grabCutContext;
        }

        util::InteractiveGrabcut *Segmentation::provideGrabCutContext(const QString &sPath, const QRectF &boundBoxRect, const QImage *qSegmMask /*= NULL*/) {
            if (NULL == _grabCutContext) {
                _grabCutContext = new util::InteractiveGrabcut();
                _grabCutContext->buildGrabCut(sPath, boundBoxRect, qSegmMask);
            }
            return _grabCutContext;
        }

        void Segmentation::saveSegmentationImage(const QString &sPath) {
            if (_shape != NULL) {
                AnnoSegmentation *segm = dynamic_cast<AnnoSegmentation *>(_shape);
                if (NULL != segm) {
                    segm->setMaskPath(sPath);
                    QImage *imgMask = segm->getMask();
                    if(NULL != imgMask) {
                        imgMask->save(sPath, "PNG");
                    }
                }
            }
        }

        void Segmentation::setSegmentationImagePath(const QString &sPath) {
            _sImagePath = sPath;
        }

        void Segmentation::buildSegmentationImage() {
            if (_shape != NULL && _sImagePath != "") {
                AnnoSegmentation *segm = dynamic_cast<AnnoSegmentation *>(_shape);
                if (NULL != segm) {
                    _grabCutContext = provideGrabCutContext();
                    _grabCutContext->buildGrabCut(_sImagePath, segm->boundingRect(), segm->getMask());
                    segm->setImage(const_cast<const QImage *>(_grabCutContext->getImageWithMask()));
                }
            }
        }

        void Segmentation::recalculateSegmentation(QRectF &newRect) {
            GlobalToolManager::instance()->recalculateSegmentation(this, newRect);
        }

        void Segmentation::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement("segmentation");
            writer.writeAttribute("uuid", annoIdAsString());
            if (!_comment.isEmpty()) {
                writer.writeTextElement("comment", _comment);
            }
            if(hasAnnoParent() || hasAnnoChildren()) {
                annoHierarchyToXml(writer);
            }
            if (!_annoClasses.isEmpty()) {
                annoClassesToXml(writer);
            }
            if (!_annoAttributes.isEmpty() || hasScore()) {
                annoAttributesToXml(writer);
            }
            if (_shape != NULL) {
                writer.writeStartElement("shape");
                _shape->toXml(writer);
                writer.writeEndElement();
            }
            writer.writeEndElement();
        }

        void Segmentation::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
            QString tagAnno("segmentation");

            if (!reader.isStartElement() || reader.name() != tagAnno) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagAnno, reader.name().toString());
            }
            _annoId = QUuid(reader.attributes().value("uuid").toString());

            XmlHelper::skipToNextStartElement(true, reader);
            if (reader.isStartElement() && reader.name() == "hierarchy") {
                loadAnnoHierarchyFromXml(reader);
                XmlHelper::skipToNextStartElement(false, reader);
            }

            if (reader.isStartElement() && reader.name() == "comment") {
                _comment = reader.readElementText();
                XmlHelper::skipToNextStartElement(true, reader);
            }
            if (reader.isStartElement() && reader.name() == "annoClass") {
                loadAnnoClassesFromXml(reader);
                XmlHelper::skipToNextStartElement(false, reader);
            }
            if (reader.isStartElement() && reader.name() == "attributeValues") {
                loadAnnoAttributesFromXml(reader);
                XmlHelper::skipToNextStartElement(false, reader);
            }

            if (!reader.isStartElement() || reader.name() != "shape") {
                throw XmlHelper::genExpFormatExpected(__FILE__, __LINE__, "shape", reader.name().toString());
            }
            _shape = AnnoShape::fromXml(reader);

            XmlHelper::skipToEndElement(tagAnno, reader);
            reader.readNext();
        }

        Annotation *Segmentation::fromXml(QXmlStreamReader &reader) throw(XmlException *) {
            Segmentation *data = new Segmentation();
            data->loadFromXml(reader);
            return data;
        }

    } //end namespace dt
}
//end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
