#ifndef ANNOFILTER_H_
#define ANNOFILTER_H_

#include <QObject>
#include <QList>
#include <QMap>
#include <QUuid>

#include "annoTypesForward.h"

namespace anno {
    namespace filter {
        class AnnoFilterRule;

        class AnnoFilter : public QObject {
                Q_OBJECT

            private:
                // Filter rules.
                QList<AnnoFilterRule *> _rules;

                // File with data to be filtered
                dt::AnnoFileData *_fileData;

                // filtered set of annotations
                QMap<QUuid, dt::Annotation *> _filteredAnnoMap;

            public:
                AnnoFilter();
                virtual ~AnnoFilter() {}

            public:
                // Applies the filter to the data.
                void apply();

                // Access stuff
            public:
                // How many annos were left after filtering?
                int filterCount() const;
                dt::Annotation *getAnnotation(const QUuid &uuid);
                const dt::Annotation *getAnnotation(const QUuid &uuid) const;
                bool containsAnnotation(const QUuid &uuid) const;
                bool containsAnnotation(const dt::Annotation *anno) const;

                QListIterator<dt::Annotation *> getAnnoIterator() const;



            signals:
                void filterSetChanged();

                //TODO signal: ergebnismenge des filters verändert
                //TODO signal: ergebnismenger verändert: added
                //TODO signal: ergebnismenger verändert: removed

        };

    }
}

#endif /* ANNOFILTER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
