#ifndef DLGFILTEREDIT_H
#define DLGFILTEREDIT_H

#include <QtGui/QDialog>
#include "../gen/ui_DlgFilterEdit.h"
#include <QItemSelection>

class AnnoFilterHighlighter;

namespace anno {
    namespace filter {
        class AnnoFilterManager;
        class AnnoFilter;
    }
}
class AnnoFiltersModelAdapter;

class DlgFilterEdit: public QDialog {
        Q_OBJECT

    private:
        static const QString NEWFILTER;

    private:
        Ui::DlgFilterEditClass ui;

    private:
        anno::filter::AnnoFilterManager *_filterMan;
        anno::filter::AnnoFilter *_curFilter;
        AnnoFiltersModelAdapter *_model;
        AnnoFilterHighlighter *_highlighter;

    private slots:
        void onLst_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
        void on_actionFilterAdd_triggered();
        void on_actionFilterRem_triggered();
        void on_btCompile_clicked();

        void on_btAnd_clicked();
        void on_btOr_clicked();
        void on_btXor_clicked();
        void on_btNot_clicked();
        void on_btClass_clicked();
        void on_btAttr_clicked();
        void on_btScore_clicked();
        void on_btSwitch_clicked();

    public:
        DlgFilterEdit(anno::filter::AnnoFilterManager *filterMan, QWidget *parent = 0);
        ~DlgFilterEdit();


};

#endif // DLGFILTEREDIT_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
