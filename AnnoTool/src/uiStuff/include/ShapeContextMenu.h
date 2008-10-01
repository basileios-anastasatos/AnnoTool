#ifndef SHAPECONTEXTMENU_H_
#define SHAPECONTEXTMENU_H_

#include <QObject>
#include <QMenu>
#include "annoTypesForward.h"
#include "RecentAttrValues.h"

class ShapeContextMenu : public QObject {
        Q_OBJECT

    private:
        anno::dt::Annotation *_curAnno;
        QMenu _menuContext;
        QMenu _menuClassAdd;
        QMenu _menuClassRem;
        QMenu _menuAttributes;
        QMenu _menuAttrValues;

        QAction *_actionAttrAdd;
        QAction *_actionAttrRem;
        QAction *_actionAttrValue;
        QAction *_lastAttrAction;

        RecentAttrValues *_recentValues;

    private:
        void initActions();
        void initMenu();
        void initClasses();
        void initAttributes();
        void initAttrValues();
        void updateClassesRemove(anno::dt::Annotation *anno);
        void updateAttributes(anno::dt::Annotation *anno);

        void internalReset();

    private slots:
        void onAction_AttrAdd();
        void onAction_AttrRem();

        void onAction_ClassAdd(QAction *action);
        void onAction_ClassRemove(QAction *action);
        void onAction_Attribute(QAction *action);
        void onMenu_AttrHovered(QAction *action);


    public:
        ShapeContextMenu(QWidget *parent = 0);
        ShapeContextMenu(const QString &title, QWidget *parent = 0);

        void triggerMenu(anno::dt::Annotation *anno);
        void setRecentValues(RecentAttrValues *recentValues);

};

#endif /* SHAPECONTEXTMENU_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
