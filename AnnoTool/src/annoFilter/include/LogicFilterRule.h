#ifndef LOGICFILTERRULE_H_
#define LOGICFILTERRULE_H_

#include "AnnoFilterRule.h"
#include <QList>

namespace anno {
    namespace filter {

        class LogicFilterRule: public AnnoFilterRule {
            protected:
                QList<AnnoFilterRule *> _rules;

            protected:
                LogicFilterRule(bool autoDelete = true);
                LogicFilterRule(bool useConst, bool constValue, bool autoDelete = true);

                static bool isXmlName(const QString &name, const QString &cur);
                virtual void toXmlInternal(QXmlStreamWriter &writer) const throw(exc::XmlException *);
                virtual void loadFromXmlInternal(const QString &xmlName, QXmlStreamReader &reader) throw(exc::XmlException *);

            public:
                virtual ~LogicFilterRule();

                // inherited interface stuff
                // ------------------------------------------------------------------------------------
                // General interface
            public:
                virtual bool isAtomic() const;
                virtual bool isValid() const;
                virtual QString toString() const;

                // Child-Access interface
            public:
                virtual bool hasChildren() const;
                virtual int childCount() const;
                virtual AnnoFilterRule *getChild(int i);
                virtual const AnnoFilterRule *getChild(int i) const;
                virtual AnnoFilterRule *takeChild(int i);
                virtual void addChild(AnnoFilterRule *child);
                virtual bool removeChild(int i);
                virtual bool removeChild(AnnoFilterRule *child);
                virtual void removeAllChildren();
                virtual bool removeDelChild(int i);
                virtual bool removeDelChild(AnnoFilterRule *child);
                virtual void removeDelAllChildren();

                // ------------------------------------------------------------------------------------
                // end: inherited interface stuff

        };

    }
}

#endif /* LOGICFILTERRULE_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
