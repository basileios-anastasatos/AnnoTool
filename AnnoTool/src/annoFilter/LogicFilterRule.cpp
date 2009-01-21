#include "include/LogicFilterRule.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AnnoFilterXmlLoader.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;

namespace anno {
    namespace filter {

        LogicFilterRule::LogicFilterRule(bool autoDelete) :
            AnnoFilterRule(autoDelete) {
        }

        LogicFilterRule::LogicFilterRule(bool useConst, bool constValue, bool autoDelete) :
            AnnoFilterRule(useConst, constValue, autoDelete) {
        }

        LogicFilterRule::~LogicFilterRule() {
            if(_autoDelete) {
                qDeleteAll(_rules);
            }
        }

        bool LogicFilterRule::isAtomic() const {
            return false;
        }

        bool LogicFilterRule::isValid() const {
            foreach(AnnoFilterRule * r, _rules) {
                if (!r->isValid()) {
                    return false;
                }
            }
            return true;
        }

        QString LogicFilterRule::toString() const {
            QString str;
            foreach(AnnoFilterRule * r, _rules) {
                str += " ";
                str += r->toString();
            }
            return str;
        }

        bool LogicFilterRule::hasChildren() const {
            return !_rules.isEmpty();
        }

        int LogicFilterRule::childCount() const {
            return _rules.size();
        }

        AnnoFilterRule *LogicFilterRule::getChild(int i) {
            if (i < 0 || i >= _rules.size()) {
                return NULL;
            }
            return _rules[i];
        }

        const AnnoFilterRule *LogicFilterRule::getChild(int i) const {
            if (i < 0 || i >= _rules.size()) {
                return NULL;
            }
            return _rules[i];
        }

        AnnoFilterRule *LogicFilterRule::takeChild(int i) {
            if (i < 0 || i >= _rules.size()) {
                return NULL;
            }
            return _rules.takeAt(i);
        }

        void LogicFilterRule::addChild(AnnoFilterRule *child) {
            _rules.append(child);
        }

        bool LogicFilterRule::removeChild(int i) {
            if (i < 0 || i >= _rules.size()) {
                return false;
            }
            _rules.removeAt(i);
            return true;
        }

        bool LogicFilterRule::removeChild(AnnoFilterRule *child) {
            return (_rules.removeAll(child) > 0);
        }

        void LogicFilterRule::removeAllChildren() {
            _rules.clear();
        }

        bool LogicFilterRule::removeDelChild(int i) {
            AnnoFilterRule *r = takeChild(i);
            if (r != NULL) {
                delete r;
                return true;
            }
            return false;
        }

        bool LogicFilterRule::removeDelChild(AnnoFilterRule *child) {
            if (removeChild(child)) {
                delete child;
                return true;
            }
            return false;
        }

        void LogicFilterRule::removeDelAllChildren() {
            qDeleteAll(_rules);
            _rules.clear();
        }

        bool LogicFilterRule::isXmlName(const QString &name, const QString &cur) {
            return (QString::compare(name, cur, Qt::CaseInsensitive) == 0);
        }

        void LogicFilterRule::toXmlInternal(QXmlStreamWriter &writer) const throw(exc::XmlException *) {
            foreach(AnnoFilterRule * r, _rules) {
                r->toXml(writer);
            }
        }

        void LogicFilterRule::loadFromXmlInternal(const QString &xmlName, QXmlStreamReader &reader) throw(exc::XmlException *) {
            while(!reader.atEnd()) {
                if(reader.isStartElement()) {
                    AnnoFilterRule *pRule = AnnoFilterXmlLoader::loadRule(reader);
                    if(pRule == NULL) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, QString("Encountered unknown Filter Rule Tag <%1>").arg(reader.name().toString()));
                    }
                    addChild(pRule);
                    continue;
                } else if(reader.isEndElement() && isXmlName(xmlName, reader.name().toString())) {
                    reader.readNext();
                    break;
                }
                reader.readNext();
            }
        }
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
