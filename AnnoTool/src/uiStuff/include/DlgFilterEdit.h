#ifndef DLGFILTEREDIT_H
#define DLGFILTEREDIT_H

#include <QDialog>
#include "../gen/ui_DlgFilterEdit.h"
#include <QItemSelection>

class AnnoFilterHighlighter;

namespace anno {
    namespace filter {
        class AnnoFilterManager;
        class AnnoFilter;
        class ColorFilterEntry;
    }
}
class AnnoFiltersModelAdapter;
class ColorFilterListModelAdapter;

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
        anno::filter::ColorFilterEntry *_curColor;
        int _curColorIndex;
        ColorFilterListModelAdapter *_colorModel;
        AnnoFilterHighlighter *_highlighter;

    private:
        void coloringSettingsEnable(bool state);

    private slots:
        void onLst_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
        void onColor_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
        void on_actionFilterAdd_triggered();
        void on_actionFilterRem_triggered();
        void on_actionColorAdd_triggered();
        void on_actionColorRem_triggered();
        void on_actionColorUp_triggered();
        void on_actionColorDown_triggered();
        void toggleGlobal(int);

        void on_ceFill_colorChanged(QColor color);
        void on_ceBorder_colorChanged(QColor color);
        void on_txtBorderWidth_editingFinished();

        void on_btCompile_clicked();
        void on_btAnd_clicked();
        void on_btOr_clicked();
        void on_btXor_clicked();
        void on_btNot_clicked();
        void on_btClass_clicked();
        void on_btAttr_clicked();
        void on_btScore_clicked();
        void on_btSwitch_clicked();

    private:

        bool isGlobal() const {
            return ui.cbIsGlobal->isChecked();
        }

        void setGlobal(bool value) {
            ui.cbIsGlobal->setChecked(value);
        }

    public:
        DlgFilterEdit(anno::filter::AnnoFilterManager *filterMan, QWidget *parent = 0);
        ~DlgFilterEdit();


};

#endif // DLGFILTEREDIT_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
