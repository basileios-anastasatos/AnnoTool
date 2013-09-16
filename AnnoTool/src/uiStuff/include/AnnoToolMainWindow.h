#ifndef ANNOTOOLMAINWINDOW_H
#define ANNOTOOLMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include "../gen/ui_AnnoToolMainWindow.h"
#include "AnnoGraphicsScene.h"
#include "GlobalToolManager.h"
#include "CopyManager.h"

#include "annoTypesForward.h"

class QCloseEvent;
class QImage;
class ZoomControl;
class FilterControl;

class AnnoToolMainWindow : public QMainWindow {
        Q_OBJECT

        // internal members
    private:
        static AnnoToolMainWindow *_me;
        Ui::AnnoToolMainWindowClass ui;
        ZoomControl *zoomCtrl;
        FilterControl *_tbFilterCtrl;
        QLabel *_lbCurImage;
        QLabel *_lbGraphicsEngine;
        anno::graphics::AnnoGraphicsScene *_graphicsScene;
        CopyManager _copyMan;

        // internal helper methods
    private:
        void clearGraphicsScene();
        void newGraphicsScene(QImage *img = NULL);
        void fitGraphicsScene();
        void loadGraphicsAnnoRaw();
        void loadGraphicsAnnoFiltered();
        void updateAnnoWidgets();
        bool checkProjectToClose();
        void configUIproject(bool open);
        void uncheckTools();
        void setToolEnabled(bool enabled);
        void lockParentAnno(bool lock = true);
        void connectFilterSignals();

        // overwritten methods
    protected:
        virtual void closeEvent(QCloseEvent *event);
        virtual void wheelEvent(QWheelEvent *event);

        // internal slots
    private slots:
        void on_actionFileNew_triggered();
        void on_actionFileOpen_triggered();
        void on_actionFileClose_triggered();
        void on_actionFileSave_triggered();
        void on_actionFileImport_triggered();
        void on_actionFileExport_triggered();
        void on_actionFileExit_triggered();
        void on_actionHelpInfo_triggered();
        void on_actionProjectDetails_triggered();
        void on_actionProjectAddImage_triggered();
        void on_actionSetImageLoader_triggered();
        void annoFileSelectChanged(int row, QUuid image);
        void on_annoListWidget_annoSelectChanged(const QModelIndex &index, QUuid anno);
        void on_actionFitImage_triggered();
        void on_actionEnableAutoFit_triggered();
        void on_actionZtoFront_triggered();
        void on_actionZtoBack_triggered();
        void on_actionRemoveAnnotation_triggered();
        void on_actionSaveCurrentImage_triggered();

        void on_actionPreviousImage_triggered();
        void on_actionNextImage_triggered();

        void on_actionCopy_triggered();
        void on_actionPaste_triggered();
        void on_actionInterpolate_triggered();

        void onZoomCtrl_zoomChanged(int value);

        // Project Manager Handling
        void onPM_projectOpened(::anno::dt::AnnoProject *project);
        void onPM_projectNew();
        void onPM_projectClosed();
        void onPM_fileListUpdate();
        void onPM_annoListUpdate();
        void onPM_annoFileSelectChanged(int row, QUuid imageId, ::anno::dt::AnnoFileData *annoFile);
        void onPM_annoSelectChanged(int row, QUuid annoId, ::anno::dt::Annotation *anno);

        // Filter handling
        void onFM_filterEnable(bool commonState, bool scoreState);
        void onFM_curFilterBegin(int preAnnoCount);
        void onFM_curFilterEnd(int preAnnoCount, int postAnnoCount);

        //Copy/Paste handling
        void onCM_dataAvailable();
        void onCM_lostData();

        // Tool support
        void on_actionLockParentAnno_triggered();
        void on_actionPoseMode_triggered();

        void on_actionToolPointer_triggered();
        void on_actionToolSinglePoint_triggered();
        void on_actionToolRectangle_triggered();
        void on_actionToolPolygon_triggered();
        void on_actionToolEllipse_triggered();
        void on_actionToolBoundingBox_triggered();
        void on_actionToolBrush_triggered();
        void on_actionGrabCut_triggered();
        void onTM_toolSelected(anno::GlobalToolManager::SelGraphicsTool tool, bool reset);

        // constrctor / destructor
    public:
        AnnoToolMainWindow(QWidget *parent = 0);
        ~AnnoToolMainWindow();

    public:
        static void updateUI();
        void openAnnoProject(const QString fileName);

        // public slot interface
    public slots:
        void onAppClose();
        void setDocumentName(const QString &name);

};

#endif // ANNOTOOLMAINWINDOW_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
