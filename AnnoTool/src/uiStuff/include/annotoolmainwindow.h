#ifndef ANNOTOOLMAINWINDOW_H
#define ANNOTOOLMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include "../gen/ui_annotoolmainwindow.h"
#include "AnnoGraphicsScene.h"

class QCloseEvent;
class QImage;
class ZoomControl;

class AnnoToolMainWindow : public QMainWindow {
        Q_OBJECT

        // internal members
    private:
        static AnnoToolMainWindow *_me;
        Ui::AnnoToolMainWindowClass ui;
        ZoomControl *zoomCtrl;
        anno::graphics::AnnoGraphicsScene *_graphicsScene;

        // internal helper methods
    private:
        void clearGraphicsScene();
        void newGraphicsScene(QImage *img = NULL);
        void fitGraphicsScene();
        void loadGraphicsAnno();
        void updateAnnoWidgets();
        bool checkProjectToClose();
        void configUIproject(bool open);
        void uncheckTools();
        void setToolEnabled(bool enabled);

        // overwritten methods
    protected:
        virtual void closeEvent(QCloseEvent *event);

        // internal slots
    private slots:
        void on_actionFileNew_triggered();
        void on_actionFileOpen_triggered();
        void on_actionFileClose_triggered();
        void on_actionFileSave_triggered();
        void on_actionFileImport_triggered();
        void on_actionFileExport_triggered();
        void on_actionFileExit_triggered();
        void on_actionProjectDetails_triggered();
        void on_actionProjectAddImage_triggered();
        void on_actionSetImageLoader_triggered();
        void annoFileSelectChanged(int row, QUuid image);
        void on_annoListWidget_annoSelectChanged(int row, QUuid anno);
        void on_zoomSlider_valueChanged(int value);
        void on_actionFitImage_triggered();

        // Tool support
        void on_actionToolPointer_triggered();
        void on_actionToolRectangle_triggered();

        // constrctor / destructor
    public:
        AnnoToolMainWindow(QWidget *parent = 0);
        ~AnnoToolMainWindow();

    public:
        static void updateUI();

        // public slot interface
    public slots:
        void on_appClose();

};

#endif // ANNOTOOLMAINWINDOW_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
