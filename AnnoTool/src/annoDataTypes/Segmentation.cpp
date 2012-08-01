#include "include/Segmentation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "importGlobals.h"
#include "AnnoBoundingBox.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

//		AnnoAttribute::AnnoAttribute(Annotation* parAnno) :
//			_modified(false), _parAnno(parAnno)
//		{
//		}
//
//		AnnoAttribute::AnnoAttribute(Annotation* parAnno, const QString& name,
//				const QString& className, const QString& value) :
//			_modified(false), _parAnno(parAnno)
//		{
//			_name = name;
//			_className = className;
//			_value = value;
//		}
//
//		AnnoAttribute::AnnoAttribute(const QString& name, const QString& className,
//				const QString& value) :
//			_modified(false), _parAnno(NULL)
//		{
//			_name = name;
//			_className = className;
//			_value = value;
//		}
//
//		void AnnoAttribute::setModified(bool mod)
//		{
//			_modified = mod;
//			if (_parAnno != NULL)
//			{
//				if (mod)
//					_parAnno->onAttrModified(this);
//				else
//					_parAnno->onAttrModifyReset(this);
//			}
//		}
//
//		void AnnoAttribute::print() const
//		{
//			QTextStream out(stdout);
//			out <<"[" <<_name <<", " <<_className <<", '" <<_value <<"']" <<endl;
//		}
//
//		bool AnnoAttribute::operator==(const AnnoAttribute& attr) const
//		{
//			return ((_name == attr._name) && (_className == attr._className));
//		}
//
//		void AnnoAttribute::toXml(QXmlStreamWriter& writer) const throw(XmlException*)
//		{
//			writer.writeStartElement("attribute");
//			writer.writeAttribute("name", _name);
//			if (!_className.isEmpty())
//			writer.writeAttribute("class", _className);
//			writer.writeCharacters(_value);
//			writer.writeEndElement();
//		}
//
//		void AnnoAttribute::loadFromXml(QXmlStreamReader& reader) throw(XmlException*)
//		{
//			QString tagAttr("attribute");
//
//			if (!reader.isStartElement() || reader.name() != tagAttr)
//			{
//				throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagAttr, reader.name().toString());
//			}
//
//			QString valName = reader.attributes().value("name").toString();
//			QString valClass = reader.attributes().value("class").toString();
//			QString val = reader.readElementText();
//
//			if (valName.isEmpty())
//			{
//				throw XmlHelper::genExpFormatAttr(__FILE__, __LINE__, "name", "!empty!");
//			}
//			_name = valName;
//			_className = valClass;
//			_value = val;
//		}
//
//		AnnoAttribute AnnoAttribute::fromXml(QXmlStreamReader& reader) throw(XmlException*)
//		{
//			AnnoAttribute data;
//			data.loadFromXml(reader);
//			return data;
//		}

        Segmentation::Segmentation(QObject *parent) { //: Annotation(parent)
            _grabCutContext = NULL;
//			_modified = false;
//			_score = NAN;
//			_shape = NULL;
//			setAllNotifications(false);
        }

        Segmentation::Segmentation(const Segmentation *anno, QObject *parent) { //: Annotation(parent)
            _grabCutContext = NULL;
//			_modified = anno->_modified;
//			_notify = anno->_notify;
//			_notifyOnChange = anno->_notifyOnChange;
//			_notifyAttr = anno->_notifyAttr;
//
//			_annoId = anno->_annoId;
//			_score = anno->_score;
//			_zOrder = anno->_zOrder;
//			_annoParent = anno->_annoParent;
//			_annoChildren = anno->_annoChildren;
//			_comment = anno->_comment;
//			_annoClasses = anno->_annoClasses;
//			_annoAttributes = anno->_annoAttributes;
//			_shape = AnnoShape::copyShape(anno->shape());
        }

        Segmentation::~Segmentation() {
//			if (_shape != NULL)
//			delete _shape;
            if (NULL != _grabCutContext) {
                delete _grabCutContext;
            }
            _grabCutContext = NULL;
        }

//		void Annotation::setModified(bool mod)
//		{
//			bool tmp = _modified;
//			_modified = mod;
//
//			if (_notify)
//			{
//				if(mod)
//				emit modified(this);
//				else
//				emit modifyReset(this);
//			}
//			if (_notifyOnChange && _modified != tmp)
//			{
//				emit modifyStateChanged(this, tmp, mod);
//			}
//		}
//
//		void Annotation::resetModifiedState(bool noNotify)
//		{
//			bool tmpNotify = _notify;
//			bool tmpNotifyOnChange = _notifyOnChange;
//			bool tmpNotifyAttr = _notifyAttr;
//
//			if (noNotify)
//			{
//				_notify = false;
//				_notifyOnChange = false;
//				_notifyAttr = false;
//			}
//
//			QMutableListIterator<AnnoAttribute> it(_annoAttributes);
//			while (it.hasNext())
//				it.next().setModified(false);
//			setModified(false);
//
//			if (noNotify)
//			{
//				_notify = tmpNotify;
//				_notifyOnChange = tmpNotifyOnChange;
//				_notifyAttr = tmpNotifyAttr;
//			}
//		}
//
//		void Annotation::onAttrModified(AnnoAttribute* attr)
//		{
//			if (attr != NULL)
//			{
//				if (_notifyAttr)
//				emit attributeModified(this, attr);
//				if (attr->isModified())
//				setModified(true);
//			}
//		}
//
//		void Annotation::onAttrModifyReset(AnnoAttribute* attr)
//		{
//			if (attr != NULL && _notifyAttr)
//			{
//				emit attributeModifyReset(this, attr);
//			}
//		}
//
//		void Annotation::print() const
//		{
//			QTextStream out(stdout);
//			out <<"Annotation..." <<endl;
//			out <<"--------------------------------------" <<endl;
//			out <<"Annotation-UUID: " <<XmlHelper::uuidAsString(_annoId) <<endl;
//			out <<"Comment: " <<_comment <<endl;
//
//			out <<endl;
//			QListIterator<QString> iClasses(_annoClasses);
//			while (iClasses.hasNext())
//			{
//				out <<"Class: " <<iClasses.next() <<endl;
//			}
//
//			out <<endl;
//			QListIterator<AnnoAttribute> iAttr(_annoAttributes);
//			while (iAttr.hasNext())
//			{
//				iAttr.next().print();
//			}
//
//			out <<endl;
//			_shape->print();
//
//			out <<"--------------------------------------" <<endl;
//		}
//
//		QString Annotation::annoInfo() const
//		{
//			QString str("%1\nscore: %2\n%3");
//			str = str.arg(annoIdAsString());
//			str = str.arg(hasScore() ? QString::number(_score, 'f', 6) : QString("--"));
//			str = str.arg(_shape != NULL ? _shape->shapeInfo() : QString("no shape"));
//			return str;
//		}
//
//		void Annotation::addClass(const QString& val)
//		{
//			if(!_annoClasses.contains(val))
//			{
//				_annoClasses.append(val);
//				setModified(true);
//			}
//		}
//
//		void Annotation::addAttribute(AnnoAttribute attr)
//		{
//			if(attr.name() != NATIVE_SCORE_ATTR && !_annoAttributes.contains(attr))
//			{
//				attr.setParentAnno(this);
//				_annoAttributes.append(attr);
//				setModified(true);
//			}
//		}
//
//		QString Annotation::getClass(int index) const
//		{
//			if(index >= 0 && index < _annoClasses.size())
//			return _annoClasses[index];
//			return QString();
//		}
//
//		AnnoAttribute* Annotation::getAttribute(int index)
//		{
//			if(index >= 0 && index < _annoAttributes.size())
//			return &_annoAttributes[index];
//			return NULL;
//		}
//
//		const AnnoAttribute* Annotation::getAttribute(int index) const
//		{
//			if(index >= 0 && index < _annoAttributes.size())
//			return &_annoAttributes[index];
//			return NULL;
//		}
//
//		void Annotation::removeClass(int index)
//		{
//			if(index >= 0 && index < _annoClasses.size())
//			{
//				_annoClasses.removeAt(index);
//				setModified(true);
//			}
//		}
//
//		void Annotation::removeClass(const QString& val)
//		{
//			//TODO delete associated attributes along with the class!?
//			int idx = _annoClasses.indexOf(val, 0);
//			if(idx >= 0)
//			{
//				_annoClasses.removeAt(idx);
//				setModified(true);
//			}
//		}
//
//		void Annotation::removeClassAll()
//		{
//			if(_annoClasses.size() > 0)
//			{
//				_annoClasses.clear();
//				setModified(true);
//			}
//		}
//
//		void Annotation::removeAttribute(int index)
//		{
//			if(index >= 0 && index < _annoAttributes.size())
//			{
//				_annoAttributes[index].setParentAnno(NULL);
//				_annoAttributes.removeAt(index);
//				setModified(true);
//			}
//		}
//
//		void Annotation::removeAttributeAll()
//		{
//			if(_annoAttributes.size() > 0)
//			{
//				_annoAttributes.clear();
//				setModified(true);
//			}
//		}

        void Segmentation::appendFGPath(const QPainterPath &fgPath) {
            _painterFGPath.addPath(fgPath);
        }

        void Segmentation::appendBGPath(const QPainterPath &bgPath) {
            _painterBGPath.addPath(bgPath);
        }

        void Segmentation::setFGPath(const QPainterPath &fgPath) {
            _painterFGPath = fgPath;
        }

        void Segmentation::setBGPath(const QPainterPath &bgPath) {
            _painterFGPath = bgPath;
        }

        const QPainterPath &Segmentation::getFGPath() {
            return _painterFGPath;
        }

        const QPainterPath &Segmentation::getBGPath() {
            return _painterBGPath;
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

//		void Annotation::annoHierarchyToXml(QXmlStreamWriter& writer) const throw(XmlException*)
//		{
//			writer.writeStartElement("hierarchy");
//			if(hasAnnoParent())
//			{
//				writer.writeEmptyElement("parent");
//				writer.writeAttribute("uuid", XmlHelper::uuidAsString(_annoParent));
//			}
//
//			if(hasAnnoChildren())
//			{
//				writer.writeStartElement("children");
//				QListIterator<QUuid> i(_annoChildren);
//				while (i.hasNext())
//				{
//					writer.writeEmptyElement("child");
//					writer.writeAttribute("uuid", XmlHelper::uuidAsString(i.next()));
//				}
//				writer.writeEndElement();
//			}
//			writer.writeEndElement();
//		}
//
//		void Annotation::annoClassesToXml(QXmlStreamWriter& writer) const
//		throw(XmlException*)
//		{
//			writer.writeStartElement("annoClass");
//			QListIterator<QString> i(_annoClasses);
//			while (i.hasNext())
//			{
//				writer.writeEmptyElement("class");
//				writer.writeAttribute("id", i.next());
//			}
//			writer.writeEndElement();
//		}
//
//		void Annotation::annoAttributesToXml(QXmlStreamWriter& writer) const
//		throw(XmlException*)
//		{
//			writer.writeStartElement("attributeValues");
//			if(hasScore())
//			{
//				AnnoAttribute sattr(NATIVE_SCORE_ATTR, QString(), QString::number(_score, 'f', 8));
//				sattr.toXml(writer);
//			}
//
//			QListIterator<AnnoAttribute> i(_annoAttributes);
//			while (i.hasNext())
//			{
//				i.next().toXml(writer);
//			}
//			writer.writeEndElement();
//		}
//
        void Segmentation::saveSegmentationImage(const QString &sPath) {
            if (_shape != NULL) {
                AnnoBoundingBox *segm = dynamic_cast<AnnoBoundingBox *>(_shape);
                if (NULL != segm) {
                    segm->setMaskPath(sPath);
                    QImage *imgMask = segm->getMask();
                    if(NULL != imgMask) {
                        imgMask->save(sPath);
                    }
                }
            }
        }

        void Segmentation::setSegmentationImagePath(const QString &sPath) {
            _sImagePath = sPath;
        }

        void Segmentation::buildSegmentationImage() {
            if (_shape != NULL && _sImagePath != "") {
                AnnoBoundingBox *segm = dynamic_cast<AnnoBoundingBox *>(_shape);
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
//
//		void Annotation::loadAnnoHierarchyFromXml(QXmlStreamReader& reader) throw(XmlException*)
//		{
//			QString tagHier("hierarchy");
//			QString tagParent("parent");
//			QString tagChildren("children");
//			QString tagChild("child");
//			QString attrId("uuid");
//
//			if (!reader.isStartElement() || reader.name() != tagHier)
//			{
//				throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagHier, reader.name().toString());
//			}
//
//			while (!reader.atEnd())
//			{
//				if (reader.isStartElement() && reader.name() == tagParent)
//				{
//					QString parent = reader.attributes().value(attrId).toString();
//					_annoParent = QUuid(parent);
//				}
//				else if (reader.isStartElement() && reader.name() == tagChild)
//				{
//					QString id = reader.attributes().value(attrId).toString();
//					_annoChildren.append(QUuid(id));
//				}
//				else if (reader.isEndElement() && reader.name() == tagHier)
//				{
//					reader.readNext();
//					break;
//				}
//				reader.readNext();
//			}
//		}
//
//		void Annotation::loadAnnoClassesFromXml(QXmlStreamReader& reader)
//		throw(XmlException*)
//		{
//			QString tagList("annoClass");
//			QString tagClass("class");
//			QString attrId("id");
//
//			if (!reader.isStartElement() || reader.name() != tagList)
//			{
//				throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagList, reader.name().toString());
//			}
//
//			while (!reader.atEnd())
//			{
//				if (reader.isStartElement() && reader.name() == tagClass)
//				{
//					QString id = reader.attributes().value(attrId).toString();
//					_annoClasses.append(id);
//				}
//				else if (reader.isEndElement() && reader.name() == tagList)
//				{
//					reader.readNext();
//					break;
//				}
//				reader.readNext();
//			}
//		}
//
//		void Annotation::loadAnnoAttributesFromXml(QXmlStreamReader& reader)
//		throw(XmlException*)
//		{
//			QString tagList("attributeValues");
//			QString tagAttr("attribute");
//
//			if (!reader.isStartElement() || reader.name() != tagList)
//			{
//				throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagList, reader.name().toString());
//			}
//
//			while (!reader.atEnd())
//			{
//				if (reader.isStartElement() && reader.name() == tagAttr)
//				{
//					AnnoAttribute attr = AnnoAttribute::fromXml(reader);
//					if(attr.className().isEmpty() && attr.name() == NATIVE_SCORE_ATTR)
//					{
//						bool ok = false;
//						double score = attr.value().toDouble(&ok);
//						if (!ok)
//						{
//							throw new XmlException(__FILE__, __LINE__, "Could not parse native score data.");
//						}
//						setScore(score);
//					}
//					else
//						addAttribute(attr);
//				}
//				else if (reader.isEndElement() && reader.name() == tagList)
//				{
//					reader.readNext();
//					break;
//				}
//				reader.readNext();
//			}
//		}
//
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
//
//          /* MA: */
//          bool Annotation::setClassAttributeValue(QString qsClass, QString qsAttribute, QString qsValue)
//          {
//            for (int idx = 0; idx < _annoAttributes.size(); ++idx) {
//              if (_annoAttributes[idx].className() == qsClass &&
//                  _annoAttributes[idx].name() == qsAttribute ) {
//                _annoAttributes[idx].setValue(qsValue);
//                return true;
//              }
//            }
//
//            return false;
//          }
//
//          bool Annotation::getClassAttributeValue(QString qsClass, QString qsAttribute, QString &qsValue)
//          {
//
//            for (int idx = 0; idx < _annoAttributes.size(); ++idx) {
//              if (_annoAttributes[idx].className() == qsClass &&
//                  _annoAttributes[idx].name() == qsAttribute ) {
//                qsValue = _annoAttributes[idx].value();
//                return true;
//              }
//            }
//
//            return false;
//          }


    } //end namespace dt
}
//end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
