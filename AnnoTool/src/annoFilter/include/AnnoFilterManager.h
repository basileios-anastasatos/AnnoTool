#ifndef ANNOFILTERMANAGER_H_
#define ANNOFILTERMANAGER_H_

#include <QObject>
#include <QList>
#include <QMap>
#include <QUuid>

#include "annoTypesForward.h"
#include "AllAnnoExceptions.h"
#include "AnnoFilter.h"

class QXmlStreamReader;
class QXmlStreamWriter;


namespace anno {
    namespace filter {

        class AnnoFilterManager : public QObject {
                Q_OBJECT

            private:
                bool _enabled;
                QMap<QString, AnnoFilter *> _filters;
                AnnoFilter *_curFilter;

                // internal helpers
            private:
                bool connectFilterSignals();
                bool disconnectFilterSignals();


            public:
                AnnoFilterManager(QObject *parent = NULL);
                virtual ~AnnoFilterManager();

                // General access interface
            public:
                int filterCount() const;
                AnnoFilter *getFilter(const QString &filterName);
                QList<AnnoFilter *> getAllFilters() const;
                AnnoFilter *curFilter();

                bool containsFilter(const QString &filterName) const;
                bool containsFilter(AnnoFilter *filter) const;

                bool addFilter(AnnoFilter *filter);
                bool removeFilter(const QString &filterName, bool delFilter = true);

            public slots:
                void setEnabled(bool enable);
                void selectFilter(const QString &filterName);
                void applyFilter();

            signals:
                void curFilterSetChanged(bool update);
                void curFilterAnnoAdded(::anno::dt::Annotation *anno);
                void curFilterAnnoRemoved(QUuid uuid);

                void filterSelected(QString filterName);
                void filterAdded(::anno::filter::AnnoFilter *filter);
                void filterRemoved(QString filterName);
                void filterEnable(bool newState);

        };

    } //end: namespace filter
} //end: namespace anno

#endif /* ANNOFILTERMANAGER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
