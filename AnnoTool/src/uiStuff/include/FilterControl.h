#ifndef FILTERCONTROL_H
#define FILTERCONTROL_H

#include <QtGui/QToolBar>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QAction>
#include "annoFiltersForward.h"

class FilterControl: public QToolBar {
        Q_OBJECT

    private:
        QAction *actionFilterEnable;
        QAction *actionFilterEdit;
        QAction *actionScoreEnable;
        QComboBox *cbFilters;
        QLineEdit *txtScore;

        bool _connected;

    private:
        void setupActions(QWidget *parent);
        void setupInputs(QWidget *parent);

    private slots:
        void on_actionFilterEnable_triggered();
        void on_actionFilterEdit_triggered();
        void on_actionScoreEnable_triggered();
        void on_cbFilters_currentIndexChanged(const QString &text);

        void onFM_filterAdded(::anno::filter::AnnoFilter *filter);
        void onFM_filterRemoved(QString filterName);

    public:
        FilterControl(QWidget *parent = 0);
        ~FilterControl();

    public slots:
        void updateFilters();
        void resetFilters();

};

#endif // FILTERCONTROL_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
