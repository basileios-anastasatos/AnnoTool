#ifndef FILTERCONTROL_H
#define FILTERCONTROL_H

#include <QToolBar>
#include <QComboBox>
#include <QLineEdit>
#include <QAction>
#include <QUuid>

#include "annoTypesForward.h"
#include "annoFiltersForward.h"

class FilterControl: public QToolBar {
        Q_OBJECT

    private:
        QAction *actionFilterEnable;
        QAction *actionFilterEdit;
        QAction *actionColorEnable;
        QAction *actionScoreEnable;
        QComboBox *cbFilters;
        QLineEdit *txtScoreLower;
        QLineEdit *txtScoreUpper;

        bool _connected;

    private:
        void setupActions(QWidget *parent);
        void setupInputs(QWidget *parent);

    private slots:
        void on_actionFilterEnable_triggered();
        void on_actionFilterEdit_triggered();
        void on_actionColorEnable_triggered();
        void on_actionScoreEnable_triggered();
        void on_cbFilters_currentIndexChanged(const QString &text);
        void on_txtScoreLower_editingFinished();
        void on_txtScoreUpper_editingFinished();

        void onFM_filterAdded(::anno::filter::AnnoFilter *filter);
        void onFM_filterRemoved(QString filterName);
        void onFM_filterEnable(bool commonState, bool scoreState);

    public:
        FilterControl(QWidget *parent = 0);
        ~FilterControl();

    public slots:
        void updateFilters();
        void resetFilters();
        void resetUi();
        void resetAll();

};

#endif // FILTERCONTROL_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
