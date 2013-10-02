#include "include/FilterControl.h"
#include "DlgFilterEdit.h"
#include "AnnoFileData.h"
#include "AnnoFilter.h"
#include "AnnoFilterManager.h"
#include "importGlobals.h"

#include <QMessageBox>

FilterControl::FilterControl(QWidget *parent) :
    QToolBar(parent), _connected(false) {
    setWindowTitle("Filter Controls");
    setObjectName("Filter Controls");
    setupActions(parent);
    setupInputs(this);
    resetUi();

    addAction(actionFilterEnable);
    addAction(actionColorEnable);
    addAction(actionFilterEdit);
    addWidget(cbFilters);
    addSeparator();
    addAction(actionScoreEnable);
    addWidget(txtScoreLower);
    addWidget(txtScoreUpper);

    //TODO remove later on
//	actionScoreEnable->setEnabled(false);
//	txtScoreLower->setEnabled(false);
//	txtScoreUpper->setEnabled(false);
}

FilterControl::~FilterControl() {

}

void FilterControl::setupActions(QWidget *parent) {
    QIcon icFilterEnable(QString::fromUtf8(":/res/filter_enable"));
    QIcon icFilterEdit(QString::fromUtf8(":/res/filter_edit"));
    QIcon icColorEnable(QString::fromUtf8(":/res/filter_enable"));
    QIcon icColorEdit(QString::fromUtf8(":/res/filter_edit"));
    QIcon icScoreEnable(QString::fromUtf8(":/res/filter_score"));

    actionFilterEnable = new QAction(parent);
    actionFilterEnable->setObjectName(QString::fromUtf8("actionFilterEnable"));
    actionFilterEnable->setCheckable(true);
    actionFilterEnable->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    actionFilterEnable->setShortcutContext(Qt::WindowShortcut);
    actionFilterEnable->setText("Enable Filter");
    actionFilterEnable->setToolTip("Enable Filter");
    actionFilterEnable->setIcon(icFilterEnable);

    actionFilterEdit = new QAction(parent);
    actionFilterEdit->setObjectName(QString::fromUtf8("actionFilterEdit"));
    actionFilterEdit->setCheckable(false);
    actionFilterEdit->setText("Edit Filters");
    actionFilterEdit->setToolTip("Edit Filters");
    actionFilterEdit->setIcon(icFilterEdit);

    actionColorEnable = new QAction(parent);
    actionColorEnable->setObjectName(QString::fromUtf8("actionColorEnable"));
    actionColorEnable->setCheckable(true);
    actionColorEnable->setText("Enable Coloring");
    actionColorEnable->setToolTip("Enable Coloring");
    actionColorEnable->setIcon(icColorEnable);

    actionScoreEnable = new QAction(parent);
    actionScoreEnable->setObjectName(QString::fromUtf8("actionScoreEnable"));
    actionScoreEnable->setCheckable(true);
    actionScoreEnable->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F));
    actionScoreEnable->setShortcutContext(Qt::WindowShortcut);
    actionScoreEnable->setText("Enable Score Filter");
    actionScoreEnable->setToolTip("Enable Score Filter");
    actionScoreEnable->setIcon(icScoreEnable);

    connect(actionFilterEnable, SIGNAL(triggered()), this, SLOT(on_actionFilterEnable_triggered()));
    connect(actionFilterEdit, SIGNAL(triggered()), this, SLOT(on_actionFilterEdit_triggered()));
    connect(actionColorEnable, SIGNAL(triggered()), this, SLOT(on_actionColorEnable_triggered()));
    connect(actionScoreEnable, SIGNAL(triggered()), this, SLOT(on_actionScoreEnable_triggered()));
}

void FilterControl::setupInputs(QWidget *parent) {
    cbFilters = new QComboBox(parent);
    cbFilters->setObjectName(QString::fromUtf8("cbFilters"));
    cbFilters->setEditable(false);
    cbFilters->setInsertPolicy(QComboBox::InsertAlphabetically);
    cbFilters->setFixedWidth(160);

    txtScoreLower = new QLineEdit(parent);
    txtScoreLower->setObjectName(QString::fromUtf8("txtScoreLower"));
    txtScoreLower->setToolTip("Lower score limit.");
    txtScoreLower->setFixedWidth(55);

    txtScoreUpper = new QLineEdit(parent);
    txtScoreUpper->setObjectName(QString::fromUtf8("txtScoreUpper"));
    txtScoreUpper->setToolTip("Upper score limit.");
    txtScoreUpper->setFixedWidth(55);

    connect(cbFilters, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_cbFilters_currentIndexChanged(const QString &)));
    connect(txtScoreLower, SIGNAL(editingFinished()), this, SLOT(on_txtScoreLower_editingFinished()));
    connect(txtScoreUpper, SIGNAL(editingFinished()), this, SLOT(on_txtScoreUpper_editingFinished()));
}

void FilterControl::on_actionFilterEnable_triggered() {
    if(GlobalProjectManager::instance()->isValid()) {
        GlobalProjectManager::instance()->filterMan()->setCommonEnabled(actionFilterEnable->isChecked());
    }
}

void FilterControl::on_actionFilterEdit_triggered() {
    if(GlobalProjectManager::instance()->isValid()) {
        DlgFilterEdit dlg(GlobalProjectManager::instance()->filterMan());
        dlg.exec();
    }
}

void FilterControl::on_actionColorEnable_triggered() {
    if(GlobalProjectManager::instance()->isValid()) {
        GlobalProjectManager::instance()->filterMan()->setColoringEnable(actionColorEnable->isChecked());
    }
}

void FilterControl::on_actionScoreEnable_triggered() {
    if(GlobalProjectManager::instance()->isValid()) {
        GlobalProjectManager::instance()->filterMan()->setScoreEnabled(actionScoreEnable->isChecked());
    }
}

void FilterControl::on_cbFilters_currentIndexChanged(const QString &text) {
    if(GlobalProjectManager::instance()->filterMan() != NULL) {
        GlobalProjectManager::instance()->filterMan()->selectFilter(text);
    }
}

void FilterControl::on_txtScoreLower_editingFinished() {
    if(GlobalProjectManager::instance()->filterMan() != NULL) {
        anno::filter::AnnoFilterManager *fm = GlobalProjectManager::instance()->filterMan();
        bool ok = false;
        double lower = txtScoreLower->text().toDouble(&ok);
        GlobalLogger::instance()->logDebug(QString("Changed lower - Actual Bounds: [%1, %2]").arg(fm->getScoreLower()).arg(fm->getScoreUpper()));
        if (ok) {
            GlobalLogger::instance()->logDebug(QString("New lower bound text: %1").arg(lower));
            if(lower > fm->getScoreUpper()) {
                QMessageBox::critical(this, "Invalid Score Bounds", "Lower score bound too high.");
            } else {
                fm->setScoreLower(lower);
            }
        } else {
            QMessageBox::critical(this, "Invalid Score Bounds", "Invalid data.");
        }
    }
}

void FilterControl::on_txtScoreUpper_editingFinished() {
    if(GlobalProjectManager::instance()->filterMan() != NULL) {
        anno::filter::AnnoFilterManager *fm = GlobalProjectManager::instance()->filterMan();
        bool ok = false;
        double upper = txtScoreUpper->text().toDouble(&ok);
        GlobalLogger::instance()->logDebug(QString("Changed upper - Actual Bounds: [%1, %2]").arg(fm->getScoreLower()).arg(fm->getScoreUpper()));
        if (ok) {
            GlobalLogger::instance()->logDebug(QString("New upper bound text: %1").arg(upper));
            if(upper < fm->getScoreLower()) {
                QMessageBox::critical(this, "Invalid Score Bounds", "Upper score bound too low.");
            } else {
                fm->setScoreUpper(upper);
            }
        } else {
            QMessageBox::critical(this, "Invalid Score Bounds", "Invalid data.");
        }
    }
}

void FilterControl::onFM_filterAdded(::anno::filter::AnnoFilter *filter) {
    cbFilters->addItem(filter->getName());
}

void FilterControl::onFM_filterRemoved(QString filterName) {
    for(int i = 0; cbFilters->count(); ++i) {
        if(cbFilters->itemText(i) == filterName) {
            cbFilters->removeItem(i);
            break;
        }
    }
}

void FilterControl::onFM_filterEnable(bool commonState, bool scoreState) {
    actionFilterEnable->setChecked(commonState);
    actionScoreEnable->setChecked(scoreState);
}

void FilterControl::updateFilters() {
    if(_connected) {
        resetFilters();
    }

    if(GlobalProjectManager::instance()->filterMan() != NULL) {
        anno::filter::AnnoFilterManager *fm = GlobalProjectManager::instance()->filterMan();
        connect(fm, SIGNAL(filterAdded(::anno::filter::AnnoFilter *)), this, SLOT(onFM_filterAdded(::anno::filter::AnnoFilter *)));
        connect(fm, SIGNAL(filterRemoved(QString)), this, SLOT(onFM_filterRemoved(QString)));
        connect(fm, SIGNAL(filterEnable(bool, bool)), this, SLOT(onFM_filterEnable(bool, bool)));
        _connected = true;

        if(fm->filterCount() > 0) {
            QList<anno::filter::AnnoFilter *> filters = fm->getAllFilters();
            for(int i = 0; i < fm->filterCount(); ++i) {
                cbFilters->addItem(filters[i]->getName());
            }
        }
    }
}

void FilterControl::resetFilters() {
    if(_connected) {
        if(GlobalProjectManager::instance()->filterMan() != NULL) {
            anno::filter::AnnoFilterManager *fm = GlobalProjectManager::instance()->filterMan();
            disconnect(fm, SIGNAL(filterAdded(::anno::filter::AnnoFilter *)), this, SLOT(onFM_filterAdded(::anno::filter::AnnoFilter *)));
            disconnect(fm, SIGNAL(filterRemoved(QString)), this, SLOT(onFM_filterRemoved(QString)));
            disconnect(fm, SIGNAL(filterEnable(bool, bool)), this, SLOT(onFM_filterEnable(bool, bool)));
        }
        _connected = false;
    }
    cbFilters->clear();
}

void FilterControl::resetUi() {
    actionFilterEnable->setChecked(false);
    actionScoreEnable->setChecked(false);
    txtScoreLower->setText("0");
    txtScoreUpper->setText("10000");
}

void FilterControl::resetAll() {
    resetFilters();
    resetUi();
}




// vim:ts=4:sts=4:sw=4:tw=80:expandtab
