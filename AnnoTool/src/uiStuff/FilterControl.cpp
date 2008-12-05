#include "include/FilterControl.h"
#include "DlgFilterEdit.h"
#include "AnnoFilter.h"
#include "AnnoFilterManager.h"
#include "importGlobals.h"

FilterControl::FilterControl(QWidget *parent) :
    QToolBar(parent), _connected(false) {
    setupActions(parent);
    setupInputs(this);

    addAction(actionFilterEnable);
    addAction(actionFilterEdit);
    addWidget(cbFilters);
    addSeparator();
    addAction(actionScoreEnable);
    addWidget(txtScore);

    //TODO remove later on
    actionScoreEnable->setEnabled(false);
    txtScore->setEnabled(false);
}

FilterControl::~FilterControl() {

}

void FilterControl::setupActions(QWidget *parent) {
    QIcon icFilterEnable(QString::fromUtf8(":/res/filter_enable"));
    QIcon icFilterEdit(QString::fromUtf8(":/res/filter_edit"));
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
    connect(actionScoreEnable, SIGNAL(triggered()), this, SLOT(on_actionScoreEnable_triggered()));
}

void FilterControl::setupInputs(QWidget *parent) {
    cbFilters = new QComboBox(parent);
    cbFilters->setObjectName(QString::fromUtf8("cbFilters"));
    cbFilters->setEditable(false);
    cbFilters->setInsertPolicy(QComboBox::InsertAlphabetically);
    cbFilters->setFixedWidth(180);

    txtScore = new QLineEdit(parent);
    txtScore->setObjectName(QString::fromUtf8("txtScore"));
    txtScore->setText("0.0");
    txtScore->setFixedWidth(72);
}

void FilterControl::on_actionFilterEnable_triggered() {
    if(GlobalProjectManager::instance()->isValid()) {
        GlobalProjectManager::instance()->filterMan()->setEnabled(actionFilterEnable->isChecked());
    }
}

void FilterControl::on_actionFilterEdit_triggered() {
    if(GlobalProjectManager::instance()->isValid()) {
        DlgFilterEdit dlg(GlobalProjectManager::instance()->filterMan());
        dlg.exec();
    }
}

void FilterControl::on_actionScoreEnable_triggered() {
    GlobalLogger::instance()->logInfo("on_actionScoreEnable_triggered");
}

void FilterControl::on_cbFilters_currentIndexChanged(const QString &text) {
    if(GlobalProjectManager::instance()->filterMan() != NULL) {
        GlobalProjectManager::instance()->filterMan()->selectFilter(text);
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

void FilterControl::updateFilters() {
    if(_connected) {
        resetFilters();
    }

    if(GlobalProjectManager::instance()->filterMan() != NULL) {
        anno::filter::AnnoFilterManager *fm = GlobalProjectManager::instance()->filterMan();
        connect(fm, SIGNAL(filterAdded(::anno::filter::AnnoFilter *)), this, SLOT(onFM_filterAdded(::anno::filter::AnnoFilter *)));
        connect(fm, SIGNAL(filterRemoved(QString)), this, SLOT(onFM_filterRemoved(QString)));

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
        }

        cbFilters->clear();
        _connected = false;
    }
}





// vim:ts=4:sts=4:sw=4:tw=80:expandtab
