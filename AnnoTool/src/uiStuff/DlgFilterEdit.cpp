#include "include/DlgFilterEdit.h"
#include "AnnoFilterManager.h"
#include "AnnoFilter.h"
#include "AnnoFilterXmlLoader.h"
#include "AnnoFiltersModelAdapter.h"
#include "importGlobals.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QModelIndex>

#include "AnnoFilterHighlighter.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;

const QString DlgFilterEdit::NEWFILTER("<New Filter>");

DlgFilterEdit::DlgFilterEdit(anno::filter::AnnoFilterManager *filterMan, QWidget *parent)
    : QDialog(parent), _filterMan(filterMan), _curFilter(NULL) {
    ui.setupUi(this);
    ui.btFiltersAdd->setDefaultAction(ui.actionFilterAdd);
    ui.btFiltersRem->setDefaultAction(ui.actionFilterRem);

    _model = new AnnoFiltersModelAdapter(_filterMan, this);
    ui.lstFilters->setModel(_model);
    _highlighter = new AnnoFilterHighlighter(ui.txtFilterRule->document());

    connect(ui.lstFilters->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onLst_selectionChanged(const QItemSelection &, const QItemSelection &)));
}

void onLst_currentChanged(const QModelIndex &current, const QModelIndex &previous);

DlgFilterEdit::~DlgFilterEdit() {

}

void DlgFilterEdit::onLst_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    QList<anno::filter::AnnoFilter *> filterList = _filterMan->getAllFilters();
    QModelIndex current = selected.indexes()[0];
    if(current.row() >= 0 && current.row() < filterList.size()) {
        _curFilter = filterList[current.row()];
        ui.txtFilterName->setText(_curFilter->getName());

        QString strXml;
        QXmlStreamWriter writer(&strXml);
        writer.setAutoFormatting(true);
        writer.setAutoFormattingIndent(2);
        try {
            if(_curFilter->hasRule()) {
                _curFilter->getFilterRule()->toXml(writer);
                writer.writeEndDocument();
            } else {
                strXml = "-- no filter rule defined --";
            }
        } catch(anno::exc::AnnoException *e) {
            QMessageBox::warning(this, "AnnoTool Filter Error", e->getTrace());
            delete e;
        }
        strXml = strXml.trimmed();
        ui.txtFilterRule->setPlainText(strXml);
    }
}

void DlgFilterEdit::on_actionFilterAdd_triggered() {
    GlobalLogger::instance()->logInfo("on_actionFilterAdd_triggered");

    anno::filter::AnnoFilter *f = new anno::filter::AnnoFilter();
    f->setName(NEWFILTER);
    if(!_filterMan->addFilter(f)) {
        delete f;
        QMessageBox::warning(this, "Anno Filter Warning", "A new filter cannot be created unless\nthe previous created filter was edited correctly.");
    } else {
        int idx = _filterMan->getAllFilters().indexOf(f);
        ui.lstFilters->selectionModel()->select(_model->index(idx), QItemSelectionModel::SelectCurrent);
        _model->update();
    }
}

void DlgFilterEdit::on_actionFilterRem_triggered() {
    GlobalLogger::instance()->logInfo("on_actionFilterRem_triggered");
}

void DlgFilterEdit::on_btCompile_clicked() {
    GlobalLogger::instance()->logInfo("on_btCompile_clicked");

    if(_curFilter != NULL) {
        QString fName = ui.txtFilterName->text();
        if(fName.isEmpty() || fName == NEWFILTER || (fName != _curFilter->getName() && _filterMan->containsFilter(fName))) {
            QMessageBox::critical(this, "Anno Filter", "Invalid filter name.\nPlease choose another filter name.");
        } else {
            QString strRule = ui.txtFilterRule->document()->toPlainText();
            GlobalLogger::instance()->logInfo(QString("XMLTEXT: ") + strRule);
            QXmlStreamReader reader(strRule);
            anno::filter::AnnoFilterRule *rule = NULL;

            if(fName != _curFilter->getName()) {
                if(_filterMan->containsFilter(fName)) {
                    QMessageBox::critical(this, "Anno Filter", "Invalid filter name.\nPlease choose another filter name.");
                } else {
                    _filterMan->removeFilter(_curFilter->getName(), false);
                    _curFilter->setName(fName);
                    _filterMan->addFilter(_curFilter);
                    _model->update();
                }
            }

            try {
                XmlHelper::skipToNextStartElement(false, reader);
                rule = anno::filter::AnnoFilterXmlLoader::loadRule(reader);
//				if(!rule->isValid())
//				{
//					QMessageBox::critical(this, "Anno Filter Error", "The current rule is invalid due to semantic error(s).");
//					delete rule;
//					return;
//				}
                _curFilter->setFilterRule(rule, true);

                QString strXml;
                QXmlStreamWriter writer(&strXml);
                writer.setAutoFormatting(true);
                writer.setAutoFormattingIndent(2);
                rule->toXml(writer);
                writer.writeEndDocument();
                strXml = strXml.trimmed();
                ui.txtFilterRule->setPlainText(strXml);
            } catch(anno::exc::AnnoException *e) {
                QMessageBox::critical(this, "Anno Filter Rule", QString("The current filter rule contains errors.\n\n") + e->getTrace());
                delete e;
                return;
            }


        }
    }


}


void DlgFilterEdit::on_btAnd_clicked() {
    ui.txtFilterRule->insertPlainText("<and></and>");
}

void DlgFilterEdit::on_btOr_clicked() {
    ui.txtFilterRule->insertPlainText("<or></or>");
}

void DlgFilterEdit::on_btXor_clicked() {
    ui.txtFilterRule->insertPlainText("<xor></xor>");
}

void DlgFilterEdit::on_btNot_clicked() {
    ui.txtFilterRule->insertPlainText("<not></not>");
}

void DlgFilterEdit::on_btClass_clicked() {
    ui.txtFilterRule->insertPlainText("<hasClass name=\"your_name\" />");
}

void DlgFilterEdit::on_btAttr_clicked() {
    ui.txtFilterRule->insertPlainText("<hasAttr name=\"your_name\" value=\"your_value\" />");
}

void DlgFilterEdit::on_btScore_clicked() {
    ui.txtFilterRule->insertPlainText("<scoreCmp value=\"your_value\" op=\"gt\" />");
}

void DlgFilterEdit::on_btSwitch_clicked() {
    ui.txtFilterRule->insertPlainText("<switch state=\"1\" />");
}




// vim:ts=4:sts=4:sw=4:tw=80:expandtab
