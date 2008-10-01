#include "include/ShapeContextMenu.h"
#include "importGlobals.h"
#include "AnnoAvClassList.h"
#include "Annotation.h"
#include "DlgEditAttributeShort.h"
#include <QAction>

#include "AnnoToolMainWindow.h"



ShapeContextMenu::ShapeContextMenu(QWidget *parent) :
    _curAnno(NULL), _menuContext(parent), _actionAttrAdd(NULL), _actionAttrRem(NULL), _actionAttrValue(NULL), _lastAttrAction(NULL), _recentValues(NULL) {
    initActions();
    initMenu();
}

ShapeContextMenu::ShapeContextMenu(const QString &title, QWidget *parent) :
    _curAnno(NULL), _menuContext(title, parent), _actionAttrAdd(NULL), _actionAttrRem(NULL), _actionAttrValue(NULL), _lastAttrAction(NULL), _recentValues(NULL) {
    initActions();
    initMenu();
}

void ShapeContextMenu::initActions() {
    _actionAttrAdd = new QAction(this);
    _actionAttrAdd->setText("Add custom attribute");

    _actionAttrRem = new QAction(this);
    _actionAttrRem->setText("Remove custom attribute");
    _actionAttrRem->setEnabled(false);

    _actionAttrValue = new QAction(this);
    _actionAttrValue->setText("Custom Value");
    _actionAttrValue->setEnabled(false);
}

void ShapeContextMenu::initMenu() {
    _menuClassAdd.setTitle("Add Annotation Class");
    _menuClassRem.setTitle("Remove Annotation Class");
    _menuAttributes.setTitle("Annotation Attributes");

    _menuContext.setTitle("Annotation Menu");
    _menuContext.addMenu(&_menuClassAdd);
    _menuContext.addMenu(&_menuClassRem);
    _menuContext.addSeparator();
    _menuContext.addMenu(&_menuAttributes);

    initClasses();

    bool connectOk = true;
    connectOk = connectOk && connect(&_menuClassAdd, SIGNAL(triggered(QAction *)), this, SLOT(onAction_ClassAdd(QAction *)));
    connectOk = connectOk && connect(&_menuClassRem, SIGNAL(triggered(QAction *)), this, SLOT(onAction_ClassRemove(QAction *)));
    connectOk = connectOk && connect(_actionAttrAdd, SIGNAL(triggered()), this, SLOT(onAction_AttrAdd()));
    connectOk = connectOk && connect(_actionAttrRem, SIGNAL(triggered()), this, SLOT(onAction_AttrRem()));
    connectOk = connectOk && connect(&_menuAttributes, SIGNAL(triggered(QAction *)), this, SLOT(onAction_Attribute(QAction *)));
    connectOk = connectOk && connect(&_menuAttributes, SIGNAL(hovered(QAction *)), this, SLOT(onMenu_AttrHovered(QAction *)));

    if(!connectOk) {
        GlobalLogger::instance()->logError("Could not connect shape context menu.");
    }
}

void ShapeContextMenu::initClasses() {
    QList<QString> classList = GlobalProjectManager::instance()->classes()->getClassNames();
    qSort(classList);
    _menuClassAdd.clear();
    foreach(QString str, classList) {
        QAction *action = new QAction(&_menuClassAdd);
        action->setText(str);
        action->setData(str);
        _menuClassAdd.addAction(action);
    }
}

void ShapeContextMenu::initAttributes() {
    _menuAttributes.clear();
    _menuAttributes.addAction(_actionAttrAdd);
    _menuAttributes.addAction(_actionAttrRem);
    _menuAttributes.addSeparator();
}

void ShapeContextMenu::initAttrValues() {
    _menuAttrValues.clear();
    _menuAttrValues.addAction(_actionAttrValue);
    _menuAttrValues.addSeparator();

    QList<QString> valList = GlobalToolManager::instance()->recentValues()->attrValues.values();
    qSort(valList);
    foreach(QString val, valList) {
        _menuAttrValues.addAction(val);
    }
}

void ShapeContextMenu::updateClassesRemove(anno::dt::Annotation *anno) {
    if(anno->classCount() > 0) {
        QList<QString> classList = anno->classes();
        qSort(classList);
        _menuClassRem.clear();
        foreach(QString str, classList) {
            QAction *action = new QAction(&_menuClassRem);
            action->setText(str);
            action->setData(str);
            _menuClassRem.addAction(action);
        }
    } else {
        QAction *action = new QAction("(no class assigned)", &_menuClassRem);
        action->setEnabled(false);
        _menuClassRem.clear();
        _menuClassRem.addAction(action);
    }
}

void ShapeContextMenu::updateAttributes(anno::dt::Annotation *anno) {
    initAttributes();

    if(anno->attributeCount() > 0 || anno->classCount() > 0) {
        //TODO attributliste

        QList<anno::dt::AnnoAttribute> attrValList = anno->attributes();
        QList<QString> curClasses = anno->classes();
        QMap<QString, anno::dt::AnnoAttribute> customAttrValues;
        QMap<QString, anno::dt::AnnoAttribute> classAttrValues;
        initAttrValues();

        foreach(anno::dt::AnnoAttribute attr, attrValList) {
            if(attr.className().isEmpty()) {
                customAttrValues[attr.name()] = attr;
            } else {
                classAttrValues[attr.name()] = attr;
            }
        }

        QMapIterator<QString, anno::dt::AnnoAttribute> it(customAttrValues);
        while(it.hasNext()) {
            it.next();
            QString entry("%1  [%2]");
            entry = entry.arg(it.key());
            if(!it.value().value().isEmpty()) {
                entry = entry.arg(it.value().value());
            } else {
                entry = entry.arg("<no value>");
            }

            QAction *action = new QAction(entry, &_menuAttributes);
            action->setMenu(&_menuAttrValues);
            _menuAttributes.addAction(action);
        }
        _menuAttributes.addSeparator();

        anno::dt::AnnoAvClassList *allClasses = GlobalProjectManager::instance()->classes();
        QMap<QString, anno::dt::AnnoAttribute> allClassAttrValues;
        foreach(QString str, curClasses) {
            anno::dt::AnnoClassDefinition *curClassDef = allClasses->getClass(str);
            QList<QString> curAttributes;
            curClassDef->allAttributes(curAttributes);
            foreach(QString attr, curAttributes) {
                allClassAttrValues.insert(QString("%1::%2").arg(curClassDef->name()).arg(attr), anno::dt::AnnoAttribute());
            }
        }

        QMapIterator<QString, anno::dt::AnnoAttribute> it2(classAttrValues);
        while(it2.hasNext()) {
            it2.next();
            allClassAttrValues.insert(QString("%1::%2").arg(it2.value().className()).arg(it2.key()), it2.value());
        }

        QMapIterator<QString, anno::dt::AnnoAttribute> it3(allClassAttrValues);
        while(it3.hasNext()) {
            it3.next();
            QString entry("%1  [%2]");
            entry = entry.arg(it3.key());
            if(!it3.value().value().isEmpty()) {
                entry = entry.arg(it3.value().value());
            } else {
                entry = entry.arg("<no value>");
            }

            QAction *action = new QAction(entry, &_menuAttributes);
            action->setMenu(&_menuAttrValues);
            _menuAttributes.addAction(action);
        }
        _menuAttributes.addSeparator();
    } else {
        QAction *action = new QAction("(no attributes)", &_menuAttributes);
        action->setEnabled(false);
        _menuAttributes.addAction(action);
    }
}

void ShapeContextMenu::internalReset() {
    _curAnno = NULL;
    _lastAttrAction = NULL;
}

void ShapeContextMenu::onAction_AttrAdd() {
    GlobalLogger::instance()->logDebug(QString(">>>>>>> on_actionAttrAdd_triggered <<<"));

    if (_curAnno != NULL) {
        DlgEditAttributeShort *dlg = new DlgEditAttributeShort();
        if (dlg->exec() == QDialog::Accepted) {
            anno::dt::AnnoAttribute attr(_curAnno);
            attr.setName(dlg->getAttrName());
            attr.setValue(dlg->getValue());
            _curAnno->addAttribute(attr);
            AnnoToolMainWindow::updateUI();
        }
        delete dlg;
    }
}

void ShapeContextMenu::onAction_AttrRem() {
    GlobalLogger::instance()->logDebug(QString(">>>>>>> on_actionAttrRem_triggered <<<"));
}

void ShapeContextMenu::onAction_ClassAdd(QAction *action) {
    GlobalLogger::instance()->logDebug(QString(">>>>>>> ADD_CLASS-ACTION: [%1] <<<").arg(action->data().toString()));

    if (_curAnno != NULL) {
        QString className = action->data().toString();
        if (!className.isEmpty()) {
            _curAnno->addClass(className);
            AnnoToolMainWindow::updateUI();
        }
    }
    internalReset();
}

void ShapeContextMenu::onAction_ClassRemove(QAction *action) {
    GlobalLogger::instance()->logDebug(QString(">>>>>>> REMOVE_CLASS-ACTION: [%1] <<<").arg(action->data().toString()));

    if (_curAnno != NULL) {
        QString className = action->data().toString();
        if(!className.isEmpty()) {
            _curAnno->removeClass(className);
            AnnoToolMainWindow::updateUI();
        }
    }
    internalReset();
}

void ShapeContextMenu::onAction_Attribute(QAction *action) {
    GlobalLogger::instance()->logDebug(QString(">>>>>>> onAction_Attribute: [%1] [%2] <<<").arg(action->text()).arg(_lastAttrAction != NULL ? _lastAttrAction->text() : "<none>"));

    if(action == _actionAttrValue) {
        if (_curAnno != NULL && _lastAttrAction != NULL) {
            DlgEditAttributeShort *dlg = new DlgEditAttributeShort();
            dlg->setNameEdit(false);
            dlg->setAttrName(_lastAttrAction->text());
            dlg->setValue("some_value");
            if (dlg->exec() == QDialog::Accepted) {
//				anno::dt::AnnoAttribute attr(_curAnno);
//				attr.setName(dlg->getAttrName());
//				attr.setValue(dlg->getValue());
//				_curAnno->addAttribute(attr);
//				AnnoToolMainWindow::updateUI();
            }
            delete dlg;
        }
    } else {

    }

    internalReset();
}

void ShapeContextMenu::onMenu_AttrHovered(QAction *action) {
    if(action->parent() == &_menuAttributes) {
        if (!action->isSeparator()) {
            _lastAttrAction = action;
        } else {
            _lastAttrAction = NULL;
        }
    }
}

void ShapeContextMenu::triggerMenu(anno::dt::Annotation *anno) {
    _curAnno = anno;
    updateClassesRemove(anno);
    updateAttributes(anno);
    _menuContext.popup(QCursor::pos());
}

void ShapeContextMenu::setRecentValues(RecentAttrValues *recentValues) {
    _recentValues = recentValues;
}




// vim:ts=4:sts=4:sw=4:tw=80:expandtab
