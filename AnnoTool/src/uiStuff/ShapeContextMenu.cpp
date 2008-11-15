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
    _actionAttrRem->setText("Remove attribute");
    // _actionAttrRem->setEnabled(false);

    _actionAttrValue = new QAction(this);
    _actionAttrValue->setText("Custom Value");
    // _actionAttrValue->setEnabled(false);
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
    connectOk = connectOk && connect(&_menuAttrValues, SIGNAL(triggered(QAction *)), this, SLOT(onAction_AttributeVal(QAction *)));
    connectOk = connectOk && connect(&_menuAttrValues, SIGNAL(aboutToShow()), this, SLOT(onMenu_AttrValShow()));
    connectOk = connectOk && connect(&_menuAttributes, SIGNAL(hovered(QAction *)), this, SLOT(onMenu_AttrHovered(QAction *)));

    if(!connectOk) {
        GlobalLogger::instance()->logError("Could not connect shape context menu.");
    }
}

void ShapeContextMenu::initClasses() {
    QList<QString> classList = GlobalProjectManager::instance()->classes()->getClassNames();
    qSort(classList);
    _menuClassAdd.clear();

    if(!classList.isEmpty()) {
        foreach(QString str, classList) {
            QAction *action = new QAction(&_menuClassAdd);
            action->setText(str);
            action->setData(str);
            _menuClassAdd.addAction(action);
        }
    } else {
        QAction *action = new QAction(&_menuClassAdd);
        action->setText("(no class data loaded)");
        action->setEnabled(false);
        _menuClassAdd.addAction(action);
    }
}

void ShapeContextMenu::initAttributes() {
    _menuAttributes.clear();
    _menuAttributes.addAction(_actionAttrAdd);
    _menuAttributes.addSeparator();
    _emptyClassAttr.clear();
}

void ShapeContextMenu::initAttrValues() {
    _menuAttrValues.clear();
    _menuAttrValues.addAction(_actionAttrValue);
    _menuAttrValues.addAction(_actionAttrRem);
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
        QList<QString> curClasses = anno->classes();
        QMap<QString, QPair<int, anno::dt::AnnoAttribute *> > customAttrValues;
        QMap<QString, QPair<int, anno::dt::AnnoAttribute *> > classAttrValues;
        initAttrValues();

        int attrCount = anno->attributeCount();
        for(int i = 0; i < attrCount; ++i) {
            anno::dt::AnnoAttribute *curAttr = anno->getAttribute(i);

            if(curAttr->className().isEmpty()) {
                customAttrValues.insert(curAttr->name(), QPair<int, anno::dt::AnnoAttribute *>(i, curAttr));
            } else {
                classAttrValues.insert(curAttr->name(), QPair<int, anno::dt::AnnoAttribute *>(i, curAttr));
            }
        }

        QMapIterator<QString, QPair<int, anno::dt::AnnoAttribute *> > it(customAttrValues);
        while(it.hasNext()) {
            it.next();
            QString entry("%1  [%2]");
            entry = entry.arg(it.key());
            if(!it.value().second->value().isEmpty()) {
                entry = entry.arg(it.value().second->value());
            } else {
                entry = entry.arg("<no value>");
            }

            QAction *action = new QAction(entry, &_menuAttributes);
            action->setData(it.value().first);
            action->setMenu(&_menuAttrValues);
            _menuAttributes.addAction(action);
        }
        _menuAttributes.addSeparator();

        anno::dt::AnnoAvClassList *allClasses = GlobalProjectManager::instance()->classes();
        QMap<QString, QPair<int, anno::dt::AnnoAttribute> > allClassAttrValues;
        foreach(QString str, curClasses) {
            anno::dt::AnnoClassDefinition *curClassDef = allClasses->getClass(str);
            QList<QString> curAttributes;
            curClassDef->allAttributes(curAttributes);
            foreach(QString attr, curAttributes) {
                allClassAttrValues.insert(QString("%1::%2").arg(curClassDef->name()).arg(attr), QPair<int, anno::dt::AnnoAttribute>(-1, anno::dt::AnnoAttribute(attr, curClassDef->name(), QString())));
            }
        }

        QMapIterator<QString, QPair<int, anno::dt::AnnoAttribute *> > it2(classAttrValues);
        while(it2.hasNext()) {
            it2.next();
            allClassAttrValues.insert(
                QString("%1::%2")
                .arg(it2.value().second->className())
                .arg(it2.key()),
                QPair<int, anno::dt::AnnoAttribute>(it2.value().first, *it2.value().second)
            );
        }

        QMapIterator<QString, QPair<int, anno::dt::AnnoAttribute> > it3(allClassAttrValues);
        int emIndex = -1;
        while(it3.hasNext()) {
            it3.next();

            QAction *action = new QAction(&_menuAttributes);
            action->setMenu(&_menuAttrValues);

            QString entry("%1  [%2]");
            entry = entry.arg(it3.key());
            if(!it3.value().second.value().isEmpty()) {
                entry = entry.arg(it3.value().second.value());
                action->setData(it3.value().first);
            } else {
                entry = entry.arg("<no value>");
                _emptyClassAttr.append(QPair<QString, QString>(it3.value().second.className(), it3.value().second.name()));
                action->setData(emIndex--);
            }
            action->setText(entry);
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

void ShapeContextMenu::doNewClassAttribute(int index, QString newValue, bool useCustom) {
    QPair<QString, QString> pair = _emptyClassAttr.at(index);
    anno::dt::AnnoAttribute newAttr;
    newAttr.setClassName(pair.first);
    newAttr.setName(pair.second);

    if(useCustom) {
        DlgEditAttributeShort dlg;
        dlg.setAttrName(newAttr.name());
        dlg.setNameEdit(false);
        if (dlg.exec() == QDialog::Accepted) {
            newValue = dlg.getValue();
        } else {
            newValue.clear();
        }
    }

    if (!newValue.isEmpty()) {
        newAttr.setValue(newValue);
        _curAnno->addAttribute(newAttr);
        AnnoToolMainWindow::updateUI();
    }
}

void ShapeContextMenu::doAttributeUpdate(int index, QString newValue, bool useCustom) {
    anno::dt::AnnoAttribute *attr = _curAnno->getAttribute(index);
    if (attr != NULL) {
        if(useCustom) {
            DlgEditAttributeShort dlg;
            dlg.setAttrName(attr->name());
            dlg.setValue(attr->value());
            dlg.setNameEdit(false);
            if (dlg.exec() == QDialog::Accepted) {
                newValue = dlg.getValue();
            } else {
                return;
            }
        }

        if (newValue.isEmpty() && !attr->className().isEmpty()) {
            _curAnno->removeAttribute(index);
        } else {
            attr->setValue(newValue);
        }

        AnnoToolMainWindow::updateUI();
    }
}

void ShapeContextMenu::onAction_AttrAdd() {
    if (_curAnno != NULL) {
        DlgEditAttributeShort dlg;
        if (dlg.exec() == QDialog::Accepted) {
            anno::dt::AnnoAttribute attr(_curAnno);
            attr.setName(dlg.getAttrName());
            attr.setValue(dlg.getValue());
            _curAnno->addAttribute(attr);
            AnnoToolMainWindow::updateUI();
        }
    }
}

void ShapeContextMenu::onAction_AttrRem() {
    if (_curAnno != NULL && _lastAttrAction != NULL) {
        bool ok = false;
        int attrIndex = _lastAttrAction->data().toInt(&ok);
        if (ok && attrIndex >= 0 && attrIndex < _curAnno->attributeCount()) {
            _curAnno->removeAttribute(attrIndex);
            AnnoToolMainWindow::updateUI();
        }
    }

    internalReset();
}

void ShapeContextMenu::onAction_ClassAdd(QAction *action) {
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
    if (_curAnno != NULL) {
        QString className = action->data().toString();
        if(!className.isEmpty()) {
            _curAnno->removeClass(className);
            AnnoToolMainWindow::updateUI();
        }
    }
    internalReset();
}

void ShapeContextMenu::onAction_AttributeVal(QAction *action) {
    if (_curAnno != NULL && _lastAttrAction != NULL) {
        bool ok = false;
        int attrIndex = _lastAttrAction->data().toInt(&ok);
        if (ok) {
            // new class attribute
            if (attrIndex < 0) {
                doNewClassAttribute(-(attrIndex + 1), action->text(), (action == _actionAttrValue));
            }
            // already existing value
            else {
                doAttributeUpdate(attrIndex, action->text(), (action == _actionAttrValue));
            }
        }
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

void ShapeContextMenu::onMenu_AttrValShow() {
    if (_curAnno != NULL && _lastAttrAction != NULL) {
        bool ok = false;
        int attrIndex = _lastAttrAction->data().toInt(&ok);
        if(ok) {
            if(attrIndex < 0) {
                _actionAttrRem->setEnabled(false);
            } else {
                _actionAttrRem->setEnabled(true);
            }
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
