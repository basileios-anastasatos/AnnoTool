#ifndef ANNOFILTER_H_
#define ANNOFILTER_H_

#include <QObject>
#include <QList>
#include <QMap>
#include <QUuid>

#include "annoTypesForward.h"
#include "AllAnnoExceptions.h"
#include "AnnoFilterRule.h"

class QXmlStreamReader;
class QXmlStreamWriter;

namespace anno {
    namespace filter {

        class AnnoFilter : public QObject {
                Q_OBJECT

                // some useful constants
            public:
                static const QString XML_SINGLEFILTER;

            private:
                // Flag indicating whether the filter is active or not.
                bool _active;

                // Flag indicating whether the filter is global or not.
                bool _global;

                // Filter name (should be unique)
                QString _name;

                // Filter comment
                QString _comment;

                // Current Filter Rule.
                AnnoFilterRule *_rule;

                // File with data to be filtered
                dt::AnnoFileData *_fileData;

                // filtered set of annotations
                QMap<QUuid, dt::Annotation *> _filteredAnnoMap;

                // internal signal connections
            private slots:
                void onAnnoAdded(::anno::dt::Annotation *anno);
                void onAnnoRemoved(QUuid uuid);
                void onAnnoUpdated(::anno::dt::AnnoFileData *annoFile, ::anno::dt::Annotation *anno);

                // internal helpers
            private:
                void invalidate();
                bool connectSignals();
                bool disconnectSignals();

            public:
                AnnoFilter(QObject *parent = NULL, bool isGlobal = false);
                AnnoFilter(AnnoFilterRule *rule, dt::AnnoFileData *fileData = NULL, QObject *parent = NULL);
                AnnoFilter(const QString &name, AnnoFilterRule *rule, dt::AnnoFileData *fileData = NULL, QObject *parent = NULL);
                AnnoFilter(const QString &name, const QString &comment, AnnoFilterRule *rule, dt::AnnoFileData *fileData = NULL, QObject *parent = NULL);
                virtual ~AnnoFilter();

                // Filter-Access stuff
            public:
                // Applies the filter to the data.
                bool applyFilter(bool partial = false);
                void resetFilter();
                bool isGlobal() const;
                void setGlobal(bool global);
                bool isActive() const;
                void deactivate();
                bool hasRule() const;

                void setName(const QString &name);
                void setComment(const QString &comment);
                void setFilterRule(AnnoFilterRule *rule, bool deleteOld = false);
                void setFileData(dt::AnnoFileData *fileData);
                QString getName() const;
                QString getComment() const;
                AnnoFilterRule *getFilterRule() const;
                dt::AnnoFileData *getFileData() const;


                // Anno-Access stuff
            public:
                // How many annos were left after filtering?
                int filterCount() const;
                dt::Annotation *getAnnotation(const QUuid &uuid);
                const dt::Annotation *getAnnotation(const QUuid &uuid) const;
                bool containsAnnotation(const QUuid &uuid) const;
                bool containsAnnotation(const dt::Annotation *anno) const;
                QList<dt::Annotation *> getAnnoList() const;

                // Copying stuff
            public:
                AnnoFilter *getCleanCopy(QObject *parent = 0) const;

                // XML interface
            public:
                void toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *);
                void loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
                static AnnoFilter *fromXml(QXmlStreamReader &reader, bool isGlobal = false) throw(exc::XmlException *);

            signals:
                void filterBegin(int preAnnoCount);
                void filterEnd(int preAnnoCount, int postAnnoCount);
                void filterSetChanged(bool update);
                void filterAnnoAdded(::anno::dt::Annotation *anno);
                void filterAnnoRemoved(QUuid uuid);

        };


        // Inlining
        // ----------------------------------------------------------------------------------
        inline bool AnnoFilter::isActive() const {
            return _active;
        }

        inline bool AnnoFilter::isGlobal() const {
            return _global;
        }

        inline void AnnoFilter::setGlobal(bool global) {
            _global = global;
        }

        inline bool AnnoFilter::hasRule() const {
            return (_rule != NULL);
        }

        inline void AnnoFilter::setName(const QString &name) {
            _name = name;
        }

        inline void AnnoFilter::setComment(const QString &comment) {
            _comment = comment;
        }

        inline void AnnoFilter::setFilterRule(AnnoFilterRule *rule, bool deleteOld) {
            if(deleteOld) {
                delete _rule;
            }
            _rule = rule;
            invalidate();
        }

        inline void AnnoFilter::setFileData(dt::AnnoFileData *fileData) {
            _fileData = fileData;
            invalidate();
        }

        inline QString AnnoFilter::getName() const {
            return _name;
        }

        inline QString AnnoFilter::getComment() const {
            return _comment;
        }

        inline AnnoFilterRule *AnnoFilter::getFilterRule() const {
            return _rule;
        }

        inline dt::AnnoFileData *AnnoFilter::getFileData() const {
            return _fileData;
        }
        // ----------------------------------------------------------------------------------

    }
}

#endif /* ANNOFILTER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
