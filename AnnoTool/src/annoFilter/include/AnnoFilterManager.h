#ifndef ANNOFILTERMANAGER_H_
#define ANNOFILTERMANAGER_H_

#include <QObject>
#include <QList>
#include <QMap>
#include <QUuid>

#include "annoTypesForward.h"
#include "annoFiltersForward.h"
#include "AllAnnoExceptions.h"
#include "AnnoFilter.h"
#include "VisualShapeConfig.h"

namespace anno {
    namespace filter {
        class ColorFilterEntry;


        class AnnoFilterManager : public QObject {
                Q_OBJECT

            private:
                bool _commonEnabled;
                bool _scoreEnabled;
                QMap<QString, AnnoFilter *> *_filters;
                AnnoFilter *_commonFilter;
                AnnoFilter *_emptyFilter;
                AnnoFilter *_scoreFilter;
                AnnoFilter *_unifiedFilter;
                AnnoFilter *_curFilter;

                AfrAnd *_scoreLink;
                AfrScoreAdv *_scoreRule;

                bool _coloringEnabled;
                QList<ColorFilterEntry *> *_colorRules;
                graphics::VisualShapeConfig _defaultShapeConfig;

                // internal helpers
            private:
                bool connectFilterSignals();
                bool disconnectFilterSignals();
                void rebuildCurrentFilter();

                // filter updating
            private slots:
                void onPM_annoFileSelectChanged(int row, QUuid imageId, ::anno::dt::AnnoFileData *annoFile);

            public:
                AnnoFilterManager(dt::AnnoProject *project = NULL, QObject *parent = NULL);
                // Complete the initialization of the object
                void setProject(dt::AnnoProject *project) throw (anno::exc::AnnoException);
                virtual ~AnnoFilterManager();

                // score filter access
            public:
                AnnoFilter *getScoreFilter();
                void setScoreLower(const double &value);
                void setScoreUpper(const double &value);
                double getScoreLower() const;
                double getScoreUpper() const;

                // General access interface
            public:
                int filterCount() const;
                AnnoFilter *getFilter(const QString &filterName);
                QList<AnnoFilter *> getAllFilters() const;
                AnnoFilter *curFilter();

                bool containsFilter(const QString &filterName) const;
                bool containsFilter(AnnoFilter *filter) const;
                bool isEnabled() const;

                bool addFilter(AnnoFilter *filter);
                bool removeFilter(const QString &filterName, bool delFilter = true);

                // Coloring interface
            public:
                int colorCount() const;
                graphics::VisualShapeConfig getColoringFor(dt::Annotation *anno) const;
                static graphics::VisualShapeConfig getDefaultColoring();

                bool isColoringEnabled() const;
                void setColoringEnable(bool state);

                ColorFilterEntry *getColorRule(int index);
                QList<ColorFilterEntry *> getAllColorRules() const;
                void addColorRule(ColorFilterEntry *rule);
                void insertColorRule(int index, ColorFilterEntry *rule);
                void removeColorRule(int index, bool delRule = true);
                void moveColorRule(int index, int newIndex);
                void incColorRule(int index);
                void decColorRule(int index);

                void updateColoring();

            public slots:
                void setCommonEnabled(bool enable);
                void setScoreEnabled(bool enable);
                void setEnabled(bool enable);
                void selectFilter(const QString &filterName);
                void applyFilter();

            signals:
                void curFilterBegin(int preAnnoCount);
                void curFilterEnd(int preAnnoCount, int postAnnoCount);
                void curFilterSetChanged(bool update);
                void curFilterAnnoAdded(::anno::dt::Annotation *anno);
                void curFilterAnnoRemoved(QUuid uuid);

                void filterSelected(QString filterName);
                void filterAdded(::anno::filter::AnnoFilter *filter);
                void filterRemoved(QString filterName);
                void filterEnable(bool commonState, bool scoreState);

                void coloringEnable(bool state);
                void coloringUpdateNeeded();
                void coloringAdded(::anno::filter::ColorFilterEntry *entry, int index);
                void coloringRemoved(QString name, int index);
                void coloringMoved(int oldIndex, int newIndex);

        };

    } //end: namespace filter
} //end: namespace anno

#endif /* ANNOFILTERMANAGER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
