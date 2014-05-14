#include "include/DlgFilterEdit.h"
#include "AnnoFilterManager.h"
#include "AnnoFilter.h"
#include "AnnoFilterXmlLoader.h"
#include "AnnoFiltersModelAdapter.h"
#include "ColorFilterListModelAdapter.h"
#include "ColorFilterEntry.h"
#include "importGlobals.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QModelIndex>
#include <QColorDialog>
#include <QImage>
#include <QPixmap>
#include <QIcon>
#include <QColor>

#include "AnnoFilterHighlighter.h"
#include "XmlHelper.h"
using anno::helper::XmlHelper;

const QString DlgFilterEdit::NEWFILTER("<New Filter>");

DlgFilterEdit::DlgFilterEdit(anno::filter::AnnoFilterManager *filterMan, QWidget *parent)
    : QDialog(parent), _filterMan(filterMan), _curFilter(NULL), _curColor(NULL), _curColorIndex(-1) {
    ui.setupUi(this);
    ui.btFiltersAdd->setDefaultAction(ui.actionFilterAdd);
    ui.btFiltersRem->setDefaultAction(ui.actionFilterRem);
    ui.btColorAdd->setDefaultAction(ui.actionColorAdd);
    ui.btColorRem->setDefaultAction(ui.actionColorRem);
    ui.btColorUp->setDefaultAction(ui.actionColorUp);
    ui.btColorDown->setDefaultAction(ui.actionColorDown);

    ui.grpFilterSettings->setEnabled(false);
    ui.grpColoring->setEnabled(false);
    coloringSettingsEnable(false);

    _model = new AnnoFiltersModelAdapter(_filterMan, this);
    _colorModel = new ColorFilterListModelAdapter(_filterMan, this);
    ui.lstFilters->setModel(_model);
    ui.lstColors->setModel(_colorModel);
    _highlighter = new AnnoFilterHighlighter(ui.txtFilterRule->document());

    connect(ui.lstFilters->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(onLst_currentRowChanged(const QModelIndex &, const QModelIndex &)));
    connect(ui.lstColors->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(onColor_currentRowChanged(const QModelIndex &, const QModelIndex &)));
    connect(ui.cbIsGlobal, SIGNAL(stateChanged(int)), this, SLOT(toggleGlobal(int)));
}

DlgFilterEdit::~DlgFilterEdit() {

}

void DlgFilterEdit::coloringSettingsEnable(bool state) {
    ui.grpColorBorder->setEnabled(state);
    ui.grpColorFill->setEnabled(state);
    ui.txtBorderWidth->setEnabled(state);
}

void DlgFilterEdit::onLst_currentRowChanged(const QModelIndex &current, const QModelIndex &previous) {
    QList<anno::filter::AnnoFilter *> filterList = _filterMan->getAllFilters();
    if(current.row() >= 0 && current.row() < filterList.size()) {
        _curFilter = filterList[current.row()];
        ui.txtFilterName->setText(_curFilter->getName());
        setGlobal(_curFilter->isGlobal());

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
        ui.grpFilterSettings->setEnabled(true);
        ui.grpColoring->setEnabled(true);
    } else {
        _curFilter = NULL;
        ui.txtFilterName->setText("");
        ui.txtFilterRule->setPlainText("");
        ui.grpFilterSettings->setEnabled(false);
    }
}

void DlgFilterEdit::onColor_currentRowChanged(const QModelIndex &current, const QModelIndex &previous) {
    if(current.row() >= 0 && current.row() < _filterMan->colorCount()) {
        _curColor = _filterMan->getColorRule(current.row());
        _curColorIndex = current.row();
        anno::graphics::VisualShapeConfig sc = _curColor->getColoring();
        ui.ceFill->setColor(sc.getFillColor());
        ui.ceBorder->setColor(sc.getBorderColor());
        ui.txtBorderWidth->setText(QString::number(sc.getBorderWidth(), 10));
        coloringSettingsEnable(true);
    } else {
        ui.ceFill->setColor(QColor(Qt::white));
        ui.ceBorder->setColor(Qt::white);
        ui.txtBorderWidth->setText("");
        coloringSettingsEnable(false);
    }
}

void DlgFilterEdit::on_actionFilterAdd_triggered() {
    anno::filter::AnnoFilter *f = new anno::filter::AnnoFilter(NULL, isGlobal());
    f->setName(NEWFILTER);
    if(!_filterMan->addFilter(f)) {
        delete f;
        QMessageBox::warning(this, "Anno Filter Warning", "A new filter cannot be created unless\nthe previous created filter was edited correctly.");
    } else {
        int idx = _filterMan->getAllFilters().indexOf(f);
        ui.lstFilters->setCurrentIndex(_model->index(idx));
    }
}

void DlgFilterEdit::toggleGlobal(int value) {
    QList<anno::filter::AnnoFilter *> filterList = _filterMan->getAllFilters();
    QModelIndex current = ui.lstFilters->currentIndex();
    if (current.row() >= 0 &&
        current.row() <  filterList.size()) {
        _curFilter = filterList[current.row()];
        _curFilter->setGlobal(isGlobal());
    }
}

void DlgFilterEdit::on_actionFilterRem_triggered() {
    if(_curFilter != NULL) {
        int selIdx = _filterMan->getAllFilters().indexOf(_curFilter);
        if(_filterMan->filterCount() <= 1) {
            selIdx = -1;
        } else if(selIdx == _filterMan->filterCount() - 1) {
            --selIdx;
        }

        _filterMan->removeFilter(_curFilter->getName(), true);
        _curFilter = NULL;
        if(selIdx >= 0) {
            ui.lstFilters->setCurrentIndex(_model->index(selIdx));
        } else {
            onLst_currentRowChanged(QModelIndex(), QModelIndex());
        }
    }
}

void DlgFilterEdit::on_actionColorAdd_triggered() {
    if(_curFilter != NULL) {
        anno::filter::ColorFilterEntry *entry = new anno::filter::ColorFilterEntry(_curFilter, anno::filter::AnnoFilterManager::getDefaultColoring());
        _filterMan->addColorRule(entry);
        int idx = _filterMan->colorCount() - 1;
        ui.lstColors->setCurrentIndex(_colorModel->index(idx));
    }
}

void DlgFilterEdit::on_actionColorRem_triggered() {
    if(_curColor != NULL) {
        int selIdx = _curColorIndex;
        if(_filterMan->colorCount() <= 1) {
            selIdx = -1;
        } else if(_curColorIndex == _filterMan->colorCount() - 1) {
            selIdx = _curColorIndex - 1;
        }

        _filterMan->removeColorRule(_curColorIndex, true);
        _curColor = NULL;
        _curColorIndex = -1;
        if(selIdx >= 0) {
            ui.lstColors->setCurrentIndex(_colorModel->index(selIdx));
        } else {
            onColor_currentRowChanged(QModelIndex(), QModelIndex());
        }
    }
}

void DlgFilterEdit::on_actionColorUp_triggered() {
    if(_curColor != NULL && _curColorIndex >= 1) {
        _filterMan->incColorRule(_curColorIndex);
        ui.lstColors->setCurrentIndex(_colorModel->index(_curColorIndex + 1));
    }
}

void DlgFilterEdit::on_actionColorDown_triggered() {
    if(_curColor != NULL && _curColorIndex < _filterMan->colorCount() - 1) {
        _filterMan->decColorRule(_curColorIndex);
        ui.lstColors->setCurrentIndex(_colorModel->index(_curColorIndex - 1));
    }
}

void DlgFilterEdit::on_ceFill_colorChanged(QColor color) {
    if(_curColor != NULL) {
        anno::graphics::VisualShapeConfig sc = _curColor->getColoring();
        sc.setFillColor(color);
        _curColor->setColoring(sc);
        _filterMan->updateColoring();
    }
}

void DlgFilterEdit::on_ceBorder_colorChanged(QColor color) {
    if(_curColor != NULL) {
        anno::graphics::VisualShapeConfig sc = _curColor->getColoring();
        sc.setBorderColor(color);
        _curColor->setColoring(sc);
        _filterMan->updateColoring();
    }
}

void DlgFilterEdit::on_txtBorderWidth_editingFinished() {
    if(_curColor != NULL) {
        bool ok = false;
        int width = ui.txtBorderWidth->text().toInt(&ok, 10);
        if(ok && width >= 0) {
            anno::graphics::VisualShapeConfig sc = _curColor->getColoring();
            sc.setBorderWidth(width);
            _curColor->setColoring(sc);
            _filterMan->updateColoring();
        }
    }
}

void DlgFilterEdit::on_btCompile_clicked() {
    if(_curFilter != NULL) {
        QString fName = ui.txtFilterName->text();
        if(fName.isEmpty() || fName == NEWFILTER || (fName != _curFilter->getName() && _filterMan->containsFilter(fName))) {
            QMessageBox::critical(this, "Anno Filter", "Invalid filter name.\nPlease choose another filter name.");
        } else {
            QString strRule = ui.txtFilterRule->document()->toPlainText();
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
