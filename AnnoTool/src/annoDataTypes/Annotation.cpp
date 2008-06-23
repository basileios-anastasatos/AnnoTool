#include "include/Annotation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "importGlobals.h"


//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoAttribute::AnnoAttribute(Annotation *parAnno) :
            _modified(false), _parAnno(parAnno) {
        }

        AnnoAttribute::AnnoAttribute(Annotation *parAnno, const QString &name,
                                     const QString &className, const QString &value) :
            _modified(false), _parAnno(parAnno) {
            _name = name;
            _className = className;
            _value = value;
        }

        AnnoAttribute::AnnoAttribute(const QString &name, const QString &className,
                                     const QString &value) :
            _modified(false), _parAnno(NULL) {
            _name = name;
            _className = className;
            _value = value;
        }

        void AnnoAttribute::setModified(bool mod) {
            _modified = mod;
            if (_parAnno != NULL) {
                if (mod) {
                    _parAnno->onAttrModified(this);
                } else {
                    _parAnno->onAttrModifyReset(this);
                }
            }
        }

        void AnnoAttribute::print() const {
            QTextStream out(stdout);
            out << "[" << _name << ", " << _className << ", '" << _value << "']" << endl;
        }

        bool AnnoAttribute::operator==(const AnnoAttribute &attr) const {
            return ((_name == attr._name) && (_className == attr._className));
        }

        void AnnoAttribute::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement("attribute");
            writer.writeAttribute("name", _name);
            if (!_className.isEmpty()) {
                writer.writeAttribute("class", _className);
            }
            writer.writeCharacters(_value);
            writer.writeEndElement();
        }

        void AnnoAttribute::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
            QString tagAttr("attribute");

            if (!reader.isStartElement() || reader.name() != tagAttr) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagAttr, reader.name().toString());
            }

            QString valName = reader.attributes().value("name").toString();
            QString valClass = reader.attributes().value("class").toString();
            QString val = reader.readElementText();

            if (valName.isEmpty()) {
                throw XmlHelper::genExpFormatAttr(__FILE__, __LINE__, "name", "!empty!");
            }
            _name = valName;
            _className = valClass;
            _value = val;
        }

        AnnoAttribute AnnoAttribute::fromXml(QXmlStreamReader &reader) throw(XmlException *) {
            AnnoAttribute data;
            data.loadFromXml(reader);
            return data;
        }

        Annotation::Annotation(QObject *parent) :
            QObject(parent), _modified(false), _score(NAN), _shape(NULL) {
            setAllNotifications(false);
        }

        Annotation::~Annotation() {
            if (_shape != NULL) {
                delete _shape;
            }
        }

        void Annotation::setModified(bool mod) {
            bool tmp = _modified;
            _modified = mod;

            if (_notify) {
                if(mod) {
                    emit modified(this);
                } else {
                    emit modifyReset(this);
                }
            }
            if (_notifyOnChange && _modified != tmp) {
                emit modifyStateChanged(this, tmp, mod);
            }
        }

        void Annotation::resetModifiedState(bool noNotify) {
            bool tmpNotify = _notify;
            bool tmpNotifyOnChange = _notifyOnChange;
            bool tmpNotifyAttr = _notifyAttr;

            if (noNotify) {
                _notify = false;
                _notifyOnChange = false;
                _notifyAttr = false;
            }

            QMutableListIterator<AnnoAttribute> it(_annoAttributes);
            while (it.hasNext()) {
                it.next().setModified(false);
            }
            setModified(false);

            if (noNotify) {
                _notify = tmpNotify;
                _notifyOnChange = tmpNotifyOnChange;
                _notifyAttr = tmpNotifyAttr;
            }
        }

        void Annotation::onAttrModified(AnnoAttribute *attr) {
            if (attr != NULL) {
                if (_notifyAttr) {
                    emit attributeModified(this, attr);
                }
                if (attr->isModified()) {
                    setModified(true);
                }
            }
        }

        void Annotation::onAttrModifyReset(AnnoAttribute *attr) {
            if (attr != NULL && _notifyAttr) {
                emit attributeModifyReset(this, attr);
            }
        }

        void Annotation::print() const {
            QTextStream out(stdout);
            out << "Annotation..." << endl;
            out << "--------------------------------------" << endl;
            out << "Annotation-UUID: " << XmlHelper::uuidAsString(_annoId) << endl;
            out << "Comment: " << _comment << endl;

            out << endl;
            QListIterator<QString> iClasses(_annoClasses);
            while (iClasses.hasNext()) {
                out << "Class: " << iClasses.next() << endl;
            }

            out << endl;
            QListIterator<AnnoAttribute> iAttr(_annoAttributes);
            while (iAttr.hasNext()) {
                iAttr.next().print();
            }

            out << endl;
            _shape->print();

            out << "--------------------------------------" << endl;
        }

        void Annotation::addClass(const QString &val) {
            if(!_annoClasses.contains(val)) {
                _annoClasses.append(val);
                setModified(true);
            }
        }

        void Annotation::addAttribute(AnnoAttribute attr) {
            if(attr.name() != NATIVE_SCORE_ATTR && !_annoAttributes.contains(attr)) {
                attr.setParentAnno(this);
                _annoAttributes.append(attr);
                setModified(true);
            }
        }

        QString Annotation::getClass(int index) const {
            if(index >= 0 && index < _annoClasses.size()) {
                return _annoClasses[index];
            }
            return QString();
        }

        AnnoAttribute *Annotation::getAttribute(int index) {
            if(index >= 0 && index < _annoAttributes.size()) {
                return &_annoAttributes[index];
            }
            return NULL;
        }

        const AnnoAttribute *Annotation::getAttribute(int index) const {
            if(index >= 0 && index < _annoAttributes.size()) {
                return &_annoAttributes[index];
            }
            return NULL;
        }

        void Annotation::removeClass(int index) {
            if(index >= 0 && index < _annoClasses.size()) {
                _annoClasses.removeAt(index);
                setModified(true);
            }
        }

        void Annotation::removeClass(const QString &val) {
            int idx = _annoClasses.indexOf(val, 0);
            if(idx >= 0) {
                _annoClasses.removeAt(idx);
                setModified(true);
            }
        }

        void Annotation::removeAttribute(int index) {
            if(index >= 0 && index < _annoAttributes.size()) {
                _annoAttributes[index].setParentAnno(NULL);
                _annoAttributes.removeAt(index);
                setModified(true);
            }
        }

        void Annotation::annoHierarchyToXml(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement("hierarchy");
            if(hasAnnoParent()) {
                writer.writeEmptyElement("parent");
                writer.writeAttribute("uuid", XmlHelper::uuidAsString(_annoParent));
            }

            if(hasAnnoChildren()) {
                writer.writeStartElement("children");
                QSetIterator<QUuid> i(_annoChildren);
                while (i.hasNext()) {
                    writer.writeEmptyElement("child");
                    writer.writeAttribute("uuid", XmlHelper::uuidAsString(i.next()));
                }
                writer.writeEndElement();
            }
            writer.writeEndElement();
        }

        void Annotation::annoClassesToXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("annoClass");
            QListIterator<QString> i(_annoClasses);
            while (i.hasNext()) {
                writer.writeEmptyElement("class");
                writer.writeAttribute("id", i.next());
            }
            writer.writeEndElement();
        }

        void Annotation::annoAttributesToXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("attributeValues");
            if(hasScore()) {
                AnnoAttribute sattr(NATIVE_SCORE_ATTR, QString(), QString::number(_score, 'f', 8));
                sattr.toXml(writer);
            }

            QListIterator<AnnoAttribute> i(_annoAttributes);
            while (i.hasNext()) {
                i.next().toXml(writer);
            }
            writer.writeEndElement();
        }

        void Annotation::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement("annotation");
            writer.writeAttribute("uuid", annoIdAsString());
            if (!_comment.isEmpty()) {
                writer.writeTextElement("comment", _comment);
            }
            if (!_annoClasses.isEmpty()) {
                annoClassesToXml(writer);
            }
            if (!_annoAttributes.isEmpty()) {
                annoAttributesToXml(writer);
            }
            if (_shape != NULL) {
                writer.writeStartElement("shape");
                _shape->toXml(writer);
                writer.writeEndElement();
            }
            writer.writeEndElement();
        }

        void Annotation::loadAnnoHierarchyFromXml(QXmlStreamReader &reader) throw(XmlException *) {
            QString tagHier("hierarchy");
            QString tagParent("parent");
            QString tagChildren("children");
            QString tagChild("child");
            QString attrId("uuid");

            if (!reader.isStartElement() || reader.name() != tagHier) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagHier, reader.name().toString());
            }

            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagParent) {
                    QString parent = reader.attributes().value(attrId).toString();
                    _annoParent = QUuid(parent);
                } else if (reader.isStartElement() && reader.name() == tagChild) {
                    QString id = reader.attributes().value(attrId).toString();
                    _annoChildren.insert(QUuid(id));
                } else if (reader.isEndElement() && reader.name() == tagHier) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }

        void Annotation::loadAnnoClassesFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            QString tagList("annoClass");
            QString tagClass("class");
            QString attrId("id");

            if (!reader.isStartElement() || reader.name() != tagList) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagList, reader.name().toString());
            }

            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagClass) {
                    QString id = reader.attributes().value(attrId).toString();
                    _annoClasses.append(id);
                } else if (reader.isEndElement() && reader.name() == tagList) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }

        void Annotation::loadAnnoAttributesFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            QString tagList("attributeValues");
            QString tagAttr("attribute");

            if (!reader.isStartElement() || reader.name() != tagList) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, tagList, reader.name().toString());
            }

            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagAttr) {
                    AnnoAttribute attr = AnnoAttribute::fromXml(reader);
                    if(attr.className().isEmpty() && attr.name() == NATIVE_SCORE_ATTR) {
                        bool ok = false;
                        double score = attr.value().toDouble(&ok);
                        if (!ok) {
                            throw new XmlException(__FILE__, __LINE__, "Could not parse native score data.");
                        }
                        setScore(score);
                    } else {
                        addAttribute(attr);
                    }
                } else if (reader.isEndElement() && reader.name() == tagList) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }

        void Annotation::loadFromXml(QXmlStreamReader &reader) throw(XmlException *) {
            QString tagAnno("annotation");

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

        Annotation *Annotation::fromXml(QXmlStreamReader &reader) throw(XmlException *) {
            Annotation *data = new Annotation();
            data->loadFromXml(reader);
            return data;
        }

    } //end namespace dt
}
//end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
