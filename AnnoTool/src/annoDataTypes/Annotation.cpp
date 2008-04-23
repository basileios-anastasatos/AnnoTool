#include "include/Annotation.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "XmlHelper.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoAttribute::AnnoAttribute() {
        }

        AnnoAttribute::AnnoAttribute(const QString &name, const QString &className,
                                     const QString &value) {
            this->name = name;
            this->className = className;
            this->value = value;
        }

        AnnoAttribute::~AnnoAttribute() {
        }

        void AnnoAttribute::print() const {
            QTextStream out(stdout);
            out << "[" << name << ", " << className << ", '" << value << "']" << endl;
        }

        void AnnoAttribute::toXml(QXmlStreamWriter &writer) const
        throw(XmlException *) {
            writer.writeStartElement("attribute");
            writer.writeAttribute("name", name);
            if (!className.isEmpty()) {
                writer.writeAttribute("class", className);
            }
            writer.writeCharacters(value);
            writer.writeEndElement();
        }

        void AnnoAttribute::loadFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
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
            name = valName;
            className = valClass;
            value = val;
        }

        AnnoAttribute AnnoAttribute::fromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            AnnoAttribute data;
            data.loadFromXml(reader);
            return data;
        }

        Annotation::Annotation() {
            _shape = NULL;
        }

        Annotation::~Annotation() {
            if (_shape != NULL) {
                delete _shape;
            }
        }

        QUuid Annotation::annoId() const {
            return _annoId;
        }

        QString Annotation::annoIdAsString() const {
            return XmlHelper::uuidAsString(_annoId);
        }

        QString Annotation::comment() const {
            return _comment;
        }

        AnnoShape *Annotation::shape() {
            return _shape;
        }

        QList<QString> *Annotation::classes() {
            return &_annoClasses;
        }

        QList<AnnoAttribute> *Annotation::attributes() {
            return &_annoAttributes;
        }

        void Annotation::setAnnoId(const QUuid &uuid) {
            _annoId = uuid;
        }

        void Annotation::setComment(const QString &comment) {
            _comment = comment;
        }

        void Annotation::setShape(AnnoShape *shape) {
            _shape = shape;
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
                    _annoAttributes.append(AnnoAttribute::fromXml(reader));
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

        Annotation *Annotation::fromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            Annotation *data = new Annotation();
            data->loadFromXml(reader);
            return data;
        }

    } //end namespace dt
}
//end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
