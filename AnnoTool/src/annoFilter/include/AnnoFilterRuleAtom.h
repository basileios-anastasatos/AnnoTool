#ifndef ANNOFILTERRULEATOM_H_
#define ANNOFILTERRULEATOM_H_

#include "AnnoFilterRule.h"

namespace anno {
    namespace filter {

        class AnnoFilterRuleAtom: public AnnoFilterRule {
            protected:
                AnnoFilterRuleAtom(bool autoDelete = true);
                AnnoFilterRuleAtom(bool useConst, bool constValue, bool autoDelete = true);

                // inherited interface stuff
                // ------------------------------------------------------------------------------------
                // General interface
            public:
                virtual bool isAtomic() const;
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

        // inlining
        // ------------------------------------------------------------------
        inline AnnoFilterRuleAtom::AnnoFilterRuleAtom(bool autoDelete) :
            AnnoFilterRule(autoDelete) {
        }

        inline AnnoFilterRuleAtom::AnnoFilterRuleAtom(bool useConst, bool constValue, bool autoDelete) :
            AnnoFilterRule(useConst, constValue, autoDelete) {
        }

        inline bool AnnoFilterRuleAtom::isAtomic() const {
            return true;
        }

        inline QString AnnoFilterRuleAtom::toString() const {
            return QString("<Abstract Atom>");
        }

        inline bool AnnoFilterRuleAtom::hasChildren() const {
            return false;
        }

        inline int AnnoFilterRuleAtom::childCount() const {
            return 0;
        }

        inline AnnoFilterRule *AnnoFilterRuleAtom::getChild(int i) {
            return NULL;
        }

        inline const AnnoFilterRule *AnnoFilterRuleAtom::getChild(int i) const {
            return NULL;
        }

        inline AnnoFilterRule *AnnoFilterRuleAtom::takeChild(int i) {
            return NULL;
        }

        inline void AnnoFilterRuleAtom::addChild(AnnoFilterRule *child) {
        }

        inline bool AnnoFilterRuleAtom::removeChild(int i) {
            return false;
        }

        inline bool AnnoFilterRuleAtom::removeChild(AnnoFilterRule *child) {
            return false;
        }

        inline void AnnoFilterRuleAtom::removeAllChildren() {
        }

        inline bool AnnoFilterRuleAtom::removeDelChild(int i) {
            return false;
        }

        inline bool AnnoFilterRuleAtom::removeDelChild(AnnoFilterRule *child) {
            return false;
        }

        inline void AnnoFilterRuleAtom::removeDelAllChildren() {
        }
        // ------------------------------------------------------------------

    }
}

#endif /* ANNOFILTERRULEATOM_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
