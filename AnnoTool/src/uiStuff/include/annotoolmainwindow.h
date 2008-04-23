#ifndef ANNOTOOLMAINWINDOW_H
#define ANNOTOOLMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include "../gen/ui_annotoolmainwindow.h"
#include "AnnoGraphicsScene.h"

class QCloseEvent;
class QImage;

class AnnoToolMainWindow : public QMainWindow {
        Q_OBJECT

    private:
        Ui::AnnoToolMainWindowClass ui;
        anno::graphics::AnnoGraphicsScene *_graphicsScene;

    private:
        void clearGraphicsScene();
        void newGraphicsScene(QImage *img = NULL);
        void loadGraphicsAnno();
        void updateAnnoWidgets();
        bool checkProjectToClose();
        void configUIproject(bool open);

    protected:
        virtual void closeEvent(QCloseEvent *event);

    private slots:
        void on_actionFileNew_triggered();
        void on_actionFileOpen_triggered();
        void on_actionFileClose_triggered();
        void on_actionFileSave_triggered();
        void on_actionFileExit_triggered();
        void on_actionProjectDetails_triggered();
        void on_actionProjectAddImage_triggered();
        void on_actionSetImageLoader_triggered();
        void annoFileSelectChanged(int row, QUuid image);
        void on_annoListWidget_annoSelectChanged(int row, QUuid anno);
        void on_zoomSlider_valueChanged(int value);

    public:
        AnnoToolMainWindow(QWidget *parent = 0);
        ~AnnoToolMainWindow();

    public slots:
        void on_appClose();

};

#endif // ANNOTOOLMAINWINDOW_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
