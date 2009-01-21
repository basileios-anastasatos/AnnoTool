#ifndef ANNOFILTERRULE_H_
#define ANNOFILTERRULE_H_

#include "annoTypesForward.h"
#include "AllAnnoExceptions.h"
#include <QString>

class QXmlStreamWriter;
class QXmlStreamReader;

namespace anno {
    namespace filter {
        /**
         * Abstract base class for all annotation filter rules.
         */
        class AnnoFilterRule {
                // protected members
            protected:
                bool _autoDelete;
                bool _useConstValue;
                bool _constValue;

                // Protected constructor makes this class abstract.
            protected:
                AnnoFilterRule(bool autoDelete = true);
                AnnoFilterRule(bool useConst, bool constValue, bool autoDelete = true);

            public:
                virtual ~AnnoFilterRule() {}

                // General interface
            public:
                bool doesAutoDelete() const;
                bool isConstActive() const;
                bool constValue() const;
                void setAutoDelete(bool autoDelete = true);
                void setConstActive(bool active = true);
                void setConstValue(bool value);

                virtual bool isAtomic() const = 0;
                virtual bool isValid() const = 0;
                virtual QString toString() const;

                // Child-Access interface
            public:
                virtual bool hasChildren() const = 0;
                virtual int childCount() const = 0;
                virtual AnnoFilterRule *getChild(int i) = 0;
                virtual const AnnoFilterRule *getChild(int i) const = 0;
                virtual AnnoFilterRule *takeChild(int i) = 0;
                virtual void addChild(AnnoFilterRule *child) = 0;
                virtual bool removeChild(int i) = 0;
                virtual bool removeChild(AnnoFilterRule *child) = 0;
                virtual void removeAllChildren() = 0;
                virtual bool removeDelChild(int i) = 0;
                virtual bool removeDelChild(AnnoFilterRule *child) = 0;
                virtual void removeDelAllChildren() = 0;

                // XML interface
            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *) = 0;
                virtual void loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) = 0;

                // Protected internal filtering interface
            protected:
                /**
                 * Tests whether the given annotation is accepted by this filter rule. That means the rule evaluates to true.
                 * Note that this method does the actual evaluation and is invoked by the public eval() method, depending on
                 * the const-value configuration.
                 *
                 * @param anno The annotation to be checked.
                 * @return True if the annotation is accepted, false otherwise.
                 * @throws exc::IllegalStateException* If the rule cannot be evaluated because of config problems or something.
                 */
                virtual bool evalInternal(const dt::Annotation *anno) const throw(exc::IllegalStateException *) = 0;

                // Public filtering interface
            public:
                /**
                 * Tests whether the given annotation is accepted by this filter rule. That means the rule evaluates to true.
                 * Note that this method respects the const-value configuration.
                 *
                 * @param anno The annotation to be checked.
                 * @return True if the annotation is accepted, false otherwise.
                 * @throws exc::IllegalStateException* If the rule cannot be evaluated because of config problems or something.
                 */
                bool eval(const dt::Annotation *anno) const throw(exc::IllegalStateException *);
        };


        // inlining
        // ------------------------------------------------------------------
        inline AnnoFilterRule::AnnoFilterRule(bool autoDelete) : _autoDelete(autoDelete), _useConstValue(false), _constValue(false) {
        }

        inline AnnoFilterRule::AnnoFilterRule(bool useConst, bool constValue, bool autoDelete) : _autoDelete(autoDelete), _useConstValue(useConst), _constValue(constValue) {
        }

        inline bool AnnoFilterRule::doesAutoDelete() const {
            return _autoDelete;
        }

        inline bool AnnoFilterRule::isConstActive() const {
            return _useConstValue;
        }

        inline bool AnnoFilterRule::constValue() const {
            return _constValue;
        }

        inline void AnnoFilterRule::setAutoDelete(bool autoDelete) {
            _autoDelete = autoDelete;
        }

        inline void AnnoFilterRule::setConstActive(bool active) {
            _useConstValue = active;
        }

        inline void AnnoFilterRule::setConstValue(bool value) {
            _constValue = value;
        }

        inline QString AnnoFilterRule::toString() const {
            return QString("<Abstract Filter Rule>");
        }
        // ------------------------------------------------------------------

    }
}

#endif /* ANNOFILTERRULE_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
