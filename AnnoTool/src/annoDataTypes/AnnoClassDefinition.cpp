#include "include/AnnoClassDefinition.h"
#include <QListIterator>
#include <QTextStream>
#include "XmlHelper.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using ::anno::helper::XmlHelper;

        AnnoClassDefinition::AnnoClassDefinition() :
            _className("noName") {
            _parent = NULL;
        }

        AnnoClassDefinition::AnnoClassDefinition(const QString &className) :
            _className(className) {
            _parent = NULL;
        }

        AnnoClassDefinition::AnnoClassDefinition(const QString &className,
                AnnoClassDefinition *parent) :
            _className(className) {
            _parent = parent;
        }

        AnnoClassDefinition::~AnnoClassDefinition() {
            _classAttributes.clear();
        }

        QString AnnoClassDefinition::name() const {
            return _className;
        }

        const AnnoClassDefinition *AnnoClassDefinition::parent() const {
            return _parent;
        }

        const QList<QString> *const AnnoClassDefinition::attributes() const {
            return &_classAttributes;
        }

        void AnnoClassDefinition::allAttributes(QList<QString> &lst) const {
            if (hasParent()) {
                _parent->allAttributes(lst);
            }

            if (!_classAttributes.isEmpty()) {
                QListIterator<QString> i(_classAttributes);
                while (i.hasNext()) {
                    lst.append(i.next());
                }
            }
        }

        bool AnnoClassDefinition::hasParent() const {
            return (_parent != NULL);
        }

        void AnnoClassDefinition::setName(QString name) {
            _className = name;
        }

        void AnnoClassDefinition::setParent(AnnoClassDefinition *parent) {
            _parent = parent;
        }

        bool AnnoClassDefinition::hasAttribute(const QString &attrName) const {
            if (_classAttributes.contains(attrName)) {
                return true;
            } else if (hasParent()) {
                return _parent->hasAttribute(attrName);
            } else {
                return false;
            }
        }

        void AnnoClassDefinition::addAttribute(const QString &attrName)
        throw(NameConflictException *) {
            if (hasAttribute(attrName)) {
                throw new NameConflictException(__FILE__, __LINE__, "Given attribute does already exist.");
            }

            _classAttributes.append(attrName);
        }

        void AnnoClassDefinition::removeAttribute(const QString &attrName) {
            _classAttributes.removeAll(attrName);
        }

        void AnnoClassDefinition::print() const {
            QTextStream out(stdout);
            out << _className;
            if (hasParent()) {
                out << " extends " << _parent->name();
            }
            out << endl << "# : " << _classAttributes.size() << endl
                << "-------------------------" << endl;
            QListIterator<QString> i(_classAttributes);
            while (i.hasNext()) {
                out << "Attribute : " << i.next() << endl;
            }
            out << "-------------------------" << endl;
        }

        void AnnoClassDefinition::toXml(QXmlStreamWriter &writer) const throw(XmlException *) {
            writer.writeStartElement("classDef");
            writer.writeAttribute("id", _className);
            if (hasParent()) {
                writer.writeAttribute("extends", _parent->name());
            }
            if (!_classAttributes.isEmpty()) {
                QListIterator<QString> i(_classAttributes);
                while (i.hasNext()) {
                    writer.writeStartElement("attribute");
                    writer.writeAttribute("name", i.next());
                    writer.writeEndElement();
                }
            }
            writer.writeEndElement();
        }

        void AnnoClassDefinition::attributesFromXml(QXmlStreamReader &reader)
        throw(XmlException *) {
            QString classTag("classDef");
            QString attrTag("attribute");
            QString attrName("name");

            if (!reader.isStartElement() || reader.name() != classTag) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, classTag, reader.name().toString());
            }

            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == attrTag) {
                    QString val = reader.attributes().value(attrName).toString();
                    addAttribute(val);
                } else if (reader.isEndElement() && reader.name() == classTag) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }

    } //end namespace dt
} //end namespace anno


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
