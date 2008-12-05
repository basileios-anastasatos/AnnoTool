#ifndef ANNOFILTERXMLLOADER_H_
#define ANNOFILTERXMLLOADER_H_

#include "AnnoFilterRule.h"
#include "AnnoFilter.h"

namespace anno {
    namespace filter {

        class AnnoFilterXmlLoader {
//			public:
//				AnnoFilterXmlLoader();
//				virtual ~AnnoFilterXmlLoader();

            public:
                static AnnoFilterRule *loadRule(QXmlStreamReader &reader) throw(exc::XmlException *);
                static AnnoFilter *loadFilter(QXmlStreamReader &reader) throw(exc::XmlException *);

        };

    }
}

#endif /* ANNOFILTERXMLLOADER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
