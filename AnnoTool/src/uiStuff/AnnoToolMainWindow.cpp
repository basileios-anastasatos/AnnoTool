#include "include/AnnoToolMainWindow.h"
#include "include/ZoomControl.h"
#include "include/FilterControl.h"
#include "include/DlgNewProject.h"
#include "include/DlgProjectDetails.h"
#include "include/DlgAddImage.h"
#include "include/DlgLoaderDetails.h"
#include "include/DlgAbout.h"
#include "include/DlgInterpolate.h"

#include "AllAnnoExceptions.h"
#include "importGlobals.h"
#include "AnnoFileData.h"
#include "AnnoOperationHelper.h"

#include "AnnoGraphicsScene.h"
#include "AnnoGraphicsPixmap.h"
#include "AnnoGraphicsShapeCreator.h"

#include "DlgImporter.h"
#include "DlgExporter.h"
#include "ImporterPlugin.h"
#include "ExporterPlugin.h"
#include "IdlExporterPlugin.h"

#include <QFileDialog>
#include <QScrollBar>
#include <QMessageBox>
#include <QApplication>
#include <QImage>
#include <QCloseEvent>

using namespace ::anno::exc;

AnnoToolMainWindow *AnnoToolMainWindow::_me = NULL;

AnnoToolMainWindow::AnnoToolMainWindow(QWidget *parent) :
    QMainWindow(parent) {
    _graphicsScene = NULL;
    ui.setupUi(this);

    setDocumentName(QString());

    setCentralWidget(ui.graphicsView);
    ui.graphicsView->setScene(&anno::graphics::AnnoGraphicsScene::EmptyScene);

    zoomCtrl = new ZoomControl(ui.tbView);
    QAction *za = ui.tbView->addWidget(zoomCtrl);
    za->setVisible(true);

    _tbFilterCtrl = new FilterControl(this);
    addToolBar(Qt::TopToolBarArea, _tbFilterCtrl);

    _lbGraphicsEngine = new QLabel(this);
    if(ui.graphicsView->isOpenGl()) {
        _lbGraphicsEngine->setText("OpenGL");
    } else {
        _lbGraphicsEngine->setText("Qt native");
    }
    _lbCurImage = new QLabel("No current Image", this);
    statusBar()->addPermanentWidget(_lbCurImage);
    statusBar()->addPermanentWidget(_lbGraphicsEngine);

    bool connectOk = true;
    connectOk = connectOk && connect(ui.annoFileListWidget, SIGNAL(annoFileSelectChanged(int, QUuid)), this, SLOT(annoFileSelectChanged(int, QUuid)));
    connectOk = connectOk && connect(zoomCtrl, SIGNAL(zoomChanged(int)), this, SLOT(onZoomCtrl_zoomChanged(int)));
    _me = this;

    GlobalProjectManager *pm = GlobalProjectManager::instance();
    connectOk = connectOk && connect(pm, SIGNAL(curAnnoModified(::anno::dt::Annotation *)), ui.annoDataWidget, SLOT(updateShapeInfo()));

    connectOk = connectOk && connect(pm, SIGNAL(projectOpened(::anno::dt::AnnoProject *)), this, SLOT(onPM_projectOpened(::anno::dt::AnnoProject *)));
    connectOk = connectOk && connect(pm, SIGNAL(projectClosed()), this, SLOT(onPM_projectClosed()));
    connectOk = connectOk && connect(pm, SIGNAL(projectNew()), this, SLOT(onPM_projectNew()));

    connectOk = connectOk && connect(pm, SIGNAL(curAnnoFileModifyStateChanged(::anno::dt::AnnoFileData *, bool, bool)), this, SLOT(onPM_fileListUpdate()));
    connectOk = connectOk && connect(pm, SIGNAL(curAnnoModifyStateChanged(::anno::dt::Annotation *, bool, bool)), this, SLOT(onPM_annoListUpdate()));
    connectOk = connectOk && connect(pm, SIGNAL(curAnnoFileSelChanged(int, QUuid, ::anno::dt::AnnoFileData *)), this, SLOT(onPM_annoFileSelectChanged(int, QUuid, ::anno::dt::AnnoFileData *)));
    connectOk = connectOk && connect(pm, SIGNAL(curAnnoSelChanged(int, QUuid, ::anno::dt::Annotation *)), this, SLOT(onPM_annoSelectChanged(int, QUuid, ::anno::dt::Annotation *)));

    connectOk = connectOk && connect(&_copyMan, SIGNAL(dataAvailable()), this, SLOT(onCM_dataAvailable()));
    connectOk = connectOk && connect(&_copyMan, SIGNAL(lostData()), this, SLOT(onCM_lostData()));

    GlobalToolManager *tm = GlobalToolManager::instance();
    connectOk = connectOk && connect(tm, SIGNAL(toolSelected(anno::GlobalToolManager::SelGraphicsTool, bool)), this, SLOT(onTM_toolSelected(anno::GlobalToolManager::SelGraphicsTool, bool)));

    if(!connectOk) {
        GlobalLogger::instance()->logError("CONNECT-ERROR: AnnoToolMainWindow::AnnoToolMainWindow(QWidget)");
    }

    configUIproject(false);
    setToolEnabled(false);

    //TODO remove this later on
    ui.actionGroundtruthMode->setVisible(false);
    ui.actionInterpolate->setEnabled(false);
}

AnnoToolMainWindow::~AnnoToolMainWindow() {

}

void AnnoToolMainWindow::clearGraphicsScene() {
    GlobalLogger::instance()->logDebug("MW: clearing graphics scene.");
    setToolEnabled(false);
    GlobalToolManager::instance()->resetAll();
    if (_graphicsScene != NULL) {
        ui.graphicsView->setScene(&anno::graphics::AnnoGraphicsScene::EmptyScene);
        delete _graphicsScene;
        _graphicsScene = NULL;
    }
}

void AnnoToolMainWindow::newGraphicsScene(QImage *img) {
    GlobalLogger::instance()->logDebug("MW: creating new graphics scene.");
    int posX = ui.graphicsView->horizontalScrollBar()->sliderPosition();
    int posY = ui.graphicsView->verticalScrollBar()->sliderPosition();
    clearGraphicsScene();
    _graphicsScene = new anno::graphics::AnnoGraphicsScene(ui.graphicsView);
    if (img != NULL) {
        _graphicsScene->setAnnoImage(*img);
    }
    ui.graphicsView->setScene(_graphicsScene);
    GlobalToolManager::instance()->setView(ui.graphicsView);
    GlobalToolManager::instance()->setScene(_graphicsScene);
    //connect(GlobalProjectManager::instance(), SIGNAL(curAnnoFile_annoAdded(::anno::dt::Annotation*)), _graphicsScene, SLOT(addAnnoShape(::anno::dt::Annotation*)));
    connect(GlobalProjectManager::instance(), SIGNAL(curAnnoFile_annoRemoved(QUuid)), _graphicsScene, SLOT(removeAnnoShape(QUuid)));
    if(ui.actionEnableAutoFit->isChecked()) {
        fitGraphicsScene();
    } else {
        zoomCtrl->setZoom(zoomCtrl->getZoom());
        ui.graphicsView->horizontalScrollBar()->setSliderPosition(posX);
        ui.graphicsView->verticalScrollBar()->setSliderPosition(posY);
    }

    connect(GlobalProjectManager::instance()->filterMan(), SIGNAL(coloringUpdateNeeded()), _graphicsScene, SLOT(applyColoring()));

    setToolEnabled(true);
}

void AnnoToolMainWindow::fitGraphicsScene() {
    if (_graphicsScene != NULL && _graphicsScene->hasImage()) {
        QSize imgSize = _graphicsScene->annoImageSize();
        int vWidth = ui.graphicsView->width() - 4;
        int vHeight = ui.graphicsView->height() - 4;
        double factor = (double)vWidth / (double)imgSize.width();
        if ((imgSize.height() * factor) > vHeight) {
            factor = (double)vHeight / (double)imgSize.height();
        }
        GlobalLogger::instance()->logDebug(QString("MW: fitting scene with factor: %1 (%2x%3 -> %4x%5)").arg(factor).arg(imgSize.width()).arg(imgSize.height()).arg(vWidth).arg(vHeight));
        zoomCtrl->setZoom(static_cast<int>(factor * 100.0));
    }
}

void AnnoToolMainWindow::loadGraphicsAnnoRaw() {
    if (_graphicsScene != NULL && GlobalProjectManager::instance()->isValid()) {
        GlobalLogger::instance()->logDebug("MW: loading annotation shapes into scene (raw).");

        anno::dt::AnnoFileData *curFile = GlobalProjectManager::instance()->selectedFile();
        if (curFile != NULL && curFile->annoCount() > 0) {
            QListIterator<anno::dt::Annotation *> i = curFile->getAnnoIterator();
            int j = 0;
            while (i.hasNext()) {
                anno::dt::Annotation *cur = i.next();
                anno::graphics::AnnoGraphicsShape *s =
                    anno::graphics::AnnoGraphicsShapeCreator::toGraphicsShape(cur);
                if (s != NULL) {
                    //GlobalLogger::instance()->logDebug(QString("MW: Adding annotation shape %1").arg(s->relatedAnno()->annoIdAsString()));
                    _graphicsScene->addAnnoShape(s);
                    ++j;
                }
            }
            GlobalLogger::instance()->logDebug(QString("Loaded %1 annotation shapes.").arg(j));
        }
    }
}

void AnnoToolMainWindow::loadGraphicsAnnoFiltered() {
    if (_graphicsScene != NULL && GlobalProjectManager::instance()->isValid() && GlobalProjectManager::instance()->filterMan()->curFilter() != NULL) {
        GlobalLogger::instance()->logDebug("MW: loading annotation shapes into scene (filtered).");

        anno::filter::AnnoFilter *filter = GlobalProjectManager::instance()->filterMan()->curFilter();
        if (filter->filterCount() > 0) {
            QList<anno::dt::Annotation *> filtered = filter->getAnnoList();
            foreach(anno::dt::Annotation * a, filtered) {
                anno::graphics::AnnoGraphicsShape *s = anno::graphics::AnnoGraphicsShapeCreator::toGraphicsShape(a);
                if (s != NULL) {
                    //GlobalLogger::instance()->logDebug(QString("MW: Adding annotation shape %1").arg(s->relatedAnno()->annoIdAsString()));
                    _graphicsScene->addAnnoShape(s);
                }
            }
            GlobalLogger::instance()->logDebug(QString("Loaded %1 annotation shapes.").arg(filtered.size()));
        }
    }
}

void AnnoToolMainWindow::updateAnnoWidgets() {
    GlobalLogger::instance()->logDebug("MW: Updating anno widgets.");
    ui.annoFileListWidget->updateData();
    ui.annoListWidget->updateData();
    ui.annoDataWidget->updateAllData();
}

bool AnnoToolMainWindow::checkProjectToClose() {
    if (GlobalProjectManager::instance()->isValid()) {
        QMessageBox::StandardButton
        ret =
            QMessageBox::question(
                this,
                "AnnoTool Project",
                "There is currently an annotation project opened.\nBy proceeding the current project will be closed and all unsaved changes will be lost!\nDo you want to proceed?",
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret == QMessageBox::No) {
            return false;
        } else {
            configUIproject(false);
            setDocumentName(QString());
            clearGraphicsScene();
            GlobalProjectManager::instance()->clear();
            return true;
        }
    }
    return true;
}

void AnnoToolMainWindow::configUIproject(bool open) {
    ui.actionFileClose->setEnabled(open);
    ui.actionFileSave->setEnabled(open);
    ui.actionFileImport->setEnabled(open);
    ui.actionFileExport->setEnabled(open);
    ui.actionProjectDetails->setEnabled(open);
    ui.actionProjectAddImage->setEnabled(open);
    ui.actionGroundtruthMode->setEnabled(open);
    _tbFilterCtrl->setEnabled(open);

    if(!open) {
        ui.actionGroundtruthMode->setChecked(false);
        _lbCurImage->setText("No current image");
        _tbFilterCtrl->resetAll();
    } else {
        _tbFilterCtrl->updateFilters();
    }
}

void AnnoToolMainWindow::uncheckTools() {
    GlobalLogger::instance()->logDebug("MW: unchecking graphics tools.");
    ui.actionToolPointer->setChecked(false);
    ui.actionToolSinglePoint->setChecked(false);
    ui.actionToolRectangle->setChecked(false);
    ui.actionToolPolygon->setChecked(false);
    ui.actionToolEllipse->setChecked(false);
}

void AnnoToolMainWindow::setToolEnabled(bool enabled) {
    GlobalLogger::instance()->logDebug(QString("MW: setting graphics tools to %1").arg(enabled ? "enabled" : "disabled"));
    ui.actionLockParentAnno->setEnabled(enabled);
    ui.actionToolPointer->setEnabled(enabled);
    ui.actionToolSinglePoint->setEnabled(enabled);
    ui.actionToolRectangle->setEnabled(enabled);
    ui.actionToolPolygon->setEnabled(enabled);
    ui.actionToolEllipse->setEnabled(enabled);

    ui.actionZtoFront->setEnabled(enabled);
    ui.actionZtoBack->setEnabled(enabled);
    ui.actionRemoveAnnotation->setEnabled(enabled);
    ui.actionSaveCurrentImage->setEnabled(enabled);
    zoomCtrl->setEnabled(enabled);
}

void AnnoToolMainWindow::lockParentAnno(bool lock) {
    // Lock currently selected annotation as parent
    if (lock) {
        if (GlobalToolManager::instance()->hasLockedAnno()) {
            GlobalLogger::instance()->logDebug("MW: cannot lock parent, there is already a lock.");
        } else {
            GlobalProjectManager *pm = GlobalProjectManager::instance();
            if (pm->isValid()) {
                QUuid curAnnoId = pm->selectedAnnoUuid();
                if (!curAnnoId.isNull()) {
                    GlobalToolManager::instance()->setLockedAnno(curAnnoId);
                    ui.actionLockParentAnno->setChecked(true);
                    GlobalLogger::instance()->logDebug("MW: locked parent");
                } else {
                    GlobalToolManager::instance()->resetLockedAnno();
                    ui.actionLockParentAnno->setChecked(false);
                    GlobalLogger::instance()->logDebug("MW: locking parent failed");
                }
            }
        }
    }
    // unlock parent annotation
    else {
        GlobalToolManager::instance()->resetLockedAnno();
        ui.actionLockParentAnno->setChecked(false);
        GlobalLogger::instance()->logDebug("MW: unlocked parent");
    }
    ui.annoListWidget->updateData();
}

void AnnoToolMainWindow::connectFilterSignals() {
    anno::filter::AnnoFilterManager *fm = GlobalProjectManager::instance()->filterMan();
    if(fm != NULL) {
        bool conOk = true;
        conOk = conOk && connect(fm, SIGNAL(filterEnable(bool, bool)), this, SLOT(onFM_filterEnable(bool, bool)));
        conOk = conOk && connect(fm, SIGNAL(curFilterBegin(int)), this, SLOT(onFM_curFilterBegin(int)));
        conOk = conOk && connect(fm, SIGNAL(curFilterEnd(int, int)), this, SLOT(onFM_curFilterEnd(int, int)));

        if(!conOk) {
            GlobalLogger::instance()->logError("CONNECT-ERROR: AnnoToolMainWindow::connectFilterSignals()");
        }
    }
}

void AnnoToolMainWindow::closeEvent(QCloseEvent *event) {
    if (checkProjectToClose()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void AnnoToolMainWindow::wheelEvent(QWheelEvent *event) {
    if(GlobalProjectManager::instance()->isValid() && (event->modifiers() & Qt::ControlModifier) != 0) {
        event->accept();
        int delta = event->delta();
        if(delta < 0) {
            ui.actionNextImage->trigger();
        } else if(delta > 0) {
            ui.actionPreviousImage->trigger();
        }
    } else if(GlobalProjectManager::instance()->isValid() && (event->modifiers() & Qt::ShiftModifier) != 0) {
        int curTool = (int)GlobalToolManager::instance()->curToolId();
        int delta = event->delta();
        if((curTool >= GlobalToolManager::GtNone) && (curTool < GlobalToolManager::GtNone2 - 1) && (delta < 0)) {
            GlobalToolManager::instance()->selectTool((GlobalToolManager::SelGraphicsTool)(curTool + 1));
        } else if(curTool <= GlobalToolManager::GtNone2 && (curTool > GlobalToolManager::GtNone + 1) && (delta > 0)) {
            GlobalToolManager::instance()->selectTool((GlobalToolManager::SelGraphicsTool)(curTool - 1));
        }

    } else if(GlobalProjectManager::instance()->isValid() && (event->modifiers() == 0)) {
        event->accept();
        int dir = (event->delta() <= 0) ? -1 : 1;
        int curZoom = zoomCtrl->getZoom();
        zoomCtrl->setZoom(curZoom + dir * 10);
    } else {
        event->ignore();
    }
}

void AnnoToolMainWindow::on_actionFileNew_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionFileNew_triggered");
    if (checkProjectToClose()) {
        DlgNewProject *dlg = new DlgNewProject(this);
        QUuid projectUuid = QUuid::createUuid();
        dlg->setProjectUuid(projectUuid);
        GlobalProjectManager *pm = GlobalProjectManager::instance();
        if (!pm->isValid() && dlg->exec() == QDialog::Accepted) {
            QFileInfo pp = dlg->projectPathAsFileInfo();
            QFileInfo dp = dlg->defaultPathAsFileInfo();

            if(dlg->makeDefaultPathRelative()) {
                QDir dir = pp.absoluteDir();
                dp = QFileInfo(dir.relativeFilePath(dp.absoluteFilePath()));
                if(dp.path().isEmpty()) {
                    dp = QFileInfo(".");
                }
            }
            GlobalLogger::instance()->logDebug(QString(">>> dp-path: %1").arg(dp.filePath()));

            pm->newEmpty(pp.filePath(), projectUuid);
            pm->project()->setProjectName(dlg->projectName());
            pm->project()->addToSearchPath(dp.filePath());
            configUIproject(true);
            setDocumentName(dlg->projectName());
            updateAnnoWidgets();
        }

        delete dlg;
    }
}

void AnnoToolMainWindow::on_actionFileOpen_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionFileOpen_triggered");

    using ::anno::dt::AnnoFileData;

    if (checkProjectToClose()) {
        QString fileName = QFileDialog::getOpenFileName(
                               this,
                               QString("Open AnnoTool Project File"),
                               QString("."),
                               QString("AnnoTool Project (%1)").arg(anno::FileExtensions::asFilter(::anno::GlobalConfig::fileExt.projects)));
        if (!fileName.isEmpty()) {
            try {
                GlobalProjectManager *pm = GlobalProjectManager::instance();
                pm->loadFromFile(fileName, true);
                GlobalLogger::instance()->logInfo(QString("Loaded project '%1': %2 classes, %3 AnnotationFiles.").arg(pm->project()->projectName()).arg(pm->classCount()).arg(pm->fileCount()));
                configUIproject(true);
                setDocumentName(pm->project()->projectName());
                updateAnnoWidgets();
                if(pm->fileCount() > 0) {
                    pm->setSelectedFileRow(0);
                }
            } catch(AnnoException *e) {
                QMessageBox::critical(this, "AnnoTool Exception", e->getTrace());
                GlobalLogger::instance()->logError(e->getTrace());
                delete e;
            }
        }
    }
}

void AnnoToolMainWindow::on_actionFileClose_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionFileClose_triggered");
    if (GlobalProjectManager::instance()->isValid()) {
        QMessageBox::StandardButton
        ret =
            QMessageBox::question(
                this,
                "AnnoTool Project",
                "You are about to close the curent project!\nBy proceeding all unsaved changes will be lost!\nDo you want to proceed?",
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret == QMessageBox::No) {
            return;
        } else {
            configUIproject(false);
            setDocumentName(QString());
            clearGraphicsScene();
            GlobalProjectManager::instance()->clear();
            updateAnnoWidgets();
        }
    }
}

void AnnoToolMainWindow::on_actionFileSave_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionFileSave_triggered");
    if (GlobalProjectManager::instance()->isValid()) {
        try {
            GlobalProjectManager::instance()->saveToFile(true);
            updateUI();
        } catch(AnnoException *e) {
            QMessageBox::critical(this, "AnnoTool Exception", e->getTrace());
            GlobalLogger::instance()->logError(e->getTrace());
            delete e;
        }
    } else {
        GlobalLogger::instance()->logError("Cannot save data, project is invalid!");
    }
}

void AnnoToolMainWindow::on_actionFileImport_triggered() {
    DlgImporter *dlg = new DlgImporter(this);
    if (dlg->exec() == QDialog::Accepted) {
        anno::ImporterPlugin *p = dlg->getSelectedImporterPtr();
        if (p != NULL) {
            GlobalLogger::instance()->logDebug(QString("Starting importer plugin [%1].").arg(p->name()));
            if (!p->exec(QFileInfo(GlobalProjectManager::instance()->projectDir(), QString()))) {
                GlobalLogger::instance()->logError(QString("Importer plugin [%1] finished execution with an error.").arg(p->name()));
            } else {
                GlobalLogger::instance()->logDebug(QString(
                                                       "Importer plugin [%1] finished successfully.").arg(p->name()));
                updateAnnoWidgets();
            }
        } else {
            GlobalLogger::instance()->logError("The selected importer plugin could not be retrieved from the ImportManager.");
        }
    }
}

void AnnoToolMainWindow::on_actionFileExport_triggered() {
    DlgExporter *dlg = new DlgExporter(this);
    if (dlg->exec() == QDialog::Accepted) {
        anno::ExporterPlugin *p = dlg->getSelectedExporterPtr();
        if (p != NULL) {
            GlobalLogger::instance()->logDebug(QString("Starting exporter plugin [%1].").arg(p->name()));
            if (!p->exec(QFileInfo(GlobalProjectManager::instance()->projectDir(), QString()))) {
                GlobalLogger::instance()->logError(QString("Exporter plugin [%1] finished execution with an error.").arg(p->name()));
            } else {
                GlobalLogger::instance()->logDebug(QString("Exporter plugin [%1] finished successfully.").arg(p->name()));
                updateAnnoWidgets();
            }
        } else {
            GlobalLogger::instance()->logError("The selected exporter plugin could not be retrieved from the ExportManager.");
        }
    }
}

void AnnoToolMainWindow::on_actionFileExit_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionFileExit_triggered");
    close();
}

void AnnoToolMainWindow::on_actionHelpInfo_triggered() {
    DlgAbout *dlg = new DlgAbout(this);
    dlg->exec();
    delete dlg;
}

void AnnoToolMainWindow::on_actionProjectDetails_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionProjectDetails_triggered");
    DlgProjectDetails dlg(this);
    dlg.exec();
}

void AnnoToolMainWindow::on_actionProjectAddImage_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionProjectAddImage_triggered");
    DlgAddImage *dlg = new DlgAddImage();
    if (dlg->exec() == QDialog::Accepted) {
        QList< QPair<QString, int> > images = dlg->images();
        if (!images.isEmpty()) {
            GlobalProjectManager *pm = GlobalProjectManager::instance();
            QFileInfo annoPath(dlg->annoSavePath());
            //			QFileInfo tmpPath = annoPath;
            if (!pm->containsInSearchPathAdv(annoPath)) {
                pm->project()->searchPath()->append(annoPath);
            }
            if (annoPath.isRelative()) {
                annoPath = pm->relToAbs(annoPath);
            }

            bool makeRel = dlg->saveImagesRel();
            QListIterator< QPair<QString, int> > i(images);
            while (i.hasNext()) {
                QPair<QString, int> img = i.next();
                QFileInfo imgPath(img.first);
                if (makeRel) {
                    imgPath = pm->absToRel(imgPath);
                }

                QUuid uuid = QUuid::createUuid();
                QString fname = GlobalProjectManager::defAnnoFileName(uuid);
                ::anno::dt::AnnoFileData *fd = new ::anno::dt::AnnoFileData(annoPath.filePath() + "/" + fname);
                fd->annoInfo()->setComplexId(pm->project()->uuid());
                ::anno::dt::AnnoImageInfo *imgInfo = fd->imageInfo();
                imgInfo->setImageId(uuid);
                imgInfo->setImagePath(imgPath);
                imgInfo->setFrame(img.second);

                pm->addAnnoFile(fd, true);
            }
            updateAnnoWidgets();
        }
    }
    delete dlg;
}

void AnnoToolMainWindow::on_actionSetImageLoader_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionSetImageLoader_triggered");
    DlgLoaderDetails *dlg = new DlgLoaderDetails(this);
    dlg->exec();
    delete dlg;
}

void AnnoToolMainWindow::onAppClose() {
    GlobalLogger::instance()->logInfo("AnnoTool is shutting down.");
    GlobalProjectManager::instance()->clear();

    try {
        GlobalLogger::instance()->logDebug("Writing anno config file.");
        GlobalConfig::instance()->saveConfig();
    } catch(AnnoException *e) {
        GlobalLogger::instance()->logError(e->msg());
        delete e;
    }
    //TODO reset singletons here!
}

void AnnoToolMainWindow::setDocumentName(const QString &name) {
#ifdef QT_DEBUG
    if(name.isEmpty()) {
        setWindowTitle(QString("%1   [Debug-Version: %2 - %3] %4").arg(GlobalInfo::appName).arg(GlobalInfo::appVersionString()).arg(GlobalInfo::compileDateTime()).arg(GlobalInfo::isExperimental ? QString("- EXPERIMENTAL Build") : QString()));
    } else {
        setWindowTitle(QString("%1  <%2>   [Debug-Version: %3 - %4] %5").arg(GlobalInfo::appName).arg(name).arg(GlobalInfo::appVersionString()).arg(GlobalInfo::compileDateTime()).arg(GlobalInfo::isExperimental ? QString("- EXPERIMENTAL Build") : QString()));
    }
#else
    if(name.isEmpty()) {
        setWindowTitle(QString("%1 %2").arg(GlobalInfo::appName).arg(GlobalInfo::isExperimental ? QString("- EXPERIMENTAL Build") : QString()));
    } else {
        setWindowTitle(QString("%1  <%2> %3").arg(GlobalInfo::appName).arg(name).arg(GlobalInfo::isExperimental ? QString("- EXPERIMENTAL Build") : QString()));
    }
#endif
}

void AnnoToolMainWindow::annoFileSelectChanged(int row, QUuid image) {
    GlobalLogger::instance()->logDebug(QString("Selected new annotation file with index %1 and UUID %2").arg(row).arg(image.toString()));
    GlobalProjectManager::instance()->setSelectedFileRow(row);
}

void AnnoToolMainWindow::on_annoListWidget_annoSelectChanged(const QModelIndex &index, QUuid anno) {
    GlobalProjectManager::instance()->setSelectedAnnoRow(anno);
//	_graphicsScene->selectShape(anno);
//	ui.annoDataWidget->updateData();
}

void AnnoToolMainWindow::onZoomCtrl_zoomChanged(int value) {
    qreal f = value / 100.0;
    statusBar()->showMessage(tr("Scale: %1").arg(f), 1000);
    ui.graphicsView->resetMatrix();
    ui.graphicsView->scale(f, f);
}

void AnnoToolMainWindow::on_actionFitImage_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionFitImage_triggered");
    fitGraphicsScene();
}

void AnnoToolMainWindow::on_actionEnableAutoFit_triggered() {

}

void AnnoToolMainWindow::on_actionZtoFront_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionZtoFront_triggered");
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if(pm->selectedAnno() != NULL) {
        //_graphicsScene->selectShape(pm->selectedAnnoUuid());
        _graphicsScene->bringSelShapeToFront();
    }
}

void AnnoToolMainWindow::on_actionZtoBack_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionZtoBack_triggered");
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if(pm->selectedAnno() != NULL) {
        //_graphicsScene->selectShape(pm->selectedAnnoUuid());
        _graphicsScene->bringSelShapeToBack();
    }
}

void AnnoToolMainWindow::on_actionRemoveAnnotation_triggered() {
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    anno::dt::AnnoFileData *curFile = pm->selectedFile();
    if(curFile != NULL) {
        anno::dt::Annotation *curAnno = pm->selectedAnno();
        if(curAnno != NULL) {
            _graphicsScene->removeAnnoShape(curAnno->annoId());
            anno::dt::AnnoOperationHelper annoOp(curFile);
            annoOp.deleteAnnotation(curAnno, false);
            updateUI();
            //pm->setSelectedAnnoRow();
        }
    }
}

void AnnoToolMainWindow::on_actionSaveCurrentImage_triggered() {
    if (_graphicsScene != NULL && !_graphicsScene->isEmpty()) {
        QString filter("JPEG-Format (*.jpg);; PNG-Format (*.png)");
        QString fileName = QFileDialog::getSaveFileName( this, tr("Save Current Image"), ".", filter);
        if (!fileName.isEmpty()) {
            QPixmap pm = _graphicsScene->annoPixmap()->pixmap();
            QImage img = pm.toImage();
            QPainter painter(&img);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::TextAntialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            _graphicsScene->render(&painter);

            if(!img.save(fileName, NULL, 100)) {
                QMessageBox::critical(this, "AnnoTool", tr("Cannot save image to %1.").arg(fileName));
            }
        }
    }
}

void AnnoToolMainWindow::on_actionPreviousImage_triggered() {
    GlobalLogger::instance()->logDebug("prev image");
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    int curRow = pm->selectedFileRow();
    if(curRow > 0) {
        pm->setSelectedFileRow(--curRow);
    }
}

void AnnoToolMainWindow::on_actionNextImage_triggered() {
    GlobalLogger::instance()->logDebug("next image");
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    int curRow = pm->selectedFileRow();
    if(curRow < pm->fileCount() - 1) {
        pm->setSelectedFileRow(++curRow);
    }
}

void AnnoToolMainWindow::on_actionCopy_triggered() {
    anno::dt::Annotation *curAnno = GlobalProjectManager::instance()->selectedAnno();
    if(curAnno != NULL) {
        _copyMan.copyAnnotation(curAnno);
    }
}

void AnnoToolMainWindow::on_actionPaste_triggered() {
    anno::dt::AnnoFileData *file = GlobalProjectManager::instance()->selectedFile();
    if(_copyMan.hasAnnotation() && file != NULL) {
        anno::dt::Annotation *anno = _copyMan.getCopyAnnotation();
        if(anno != NULL) {
            if(!file->containsAnnotation(anno)) {
                file->addAnnotation(anno);
                _graphicsScene->addAnnoShape(anno);
                updateAnnoWidgets();
            }
        }
    }
}

void AnnoToolMainWindow::on_actionInterpolate_triggered() {
    DlgInterpolate dlg(this);
    dlg.setVisible(true);
}

void AnnoToolMainWindow::onPM_projectOpened(::anno::dt::AnnoProject *project) {
    connectFilterSignals();
}

void AnnoToolMainWindow::onPM_projectNew() {
    connectFilterSignals();
}

void AnnoToolMainWindow::onPM_projectClosed() {

}

void AnnoToolMainWindow::onPM_fileListUpdate() {
    GlobalLogger::instance()->logDebug("MW: onPM_fileListUpdate");
    ui.annoFileListWidget->updateData();
}

void AnnoToolMainWindow::onPM_annoListUpdate() {
    GlobalLogger::instance()->logDebug("MW: onPM_annoListUpdate");
    ui.annoListWidget->updateData();
}

void AnnoToolMainWindow::onPM_annoFileSelectChanged(int row, QUuid imageId, ::anno::dt::AnnoFileData *annoFile) {
    GlobalLogger::instance()->logDebug("MW: onPM_annoFileSelectChanged");
    setToolEnabled(false);
    lockParentAnno(false);
    _lbCurImage->setText(QString());

    ui.annoListWidget->updateData();
    ui.annoDataWidget->updateAllData();

    //TODO check correctness!
    if(row < 0) {
        return;
    }

    QFileInfo fileName = annoFile->imageInfo()->imagePath();
    int frame = annoFile->imageInfo()->frame();
    if (fileName.isRelative()) {
        fileName = GlobalProjectManager::instance()->relToAbs(fileName);
    }
    GlobalLogger::instance()->logDebug(QString("Loading image [%1]").arg(fileName.filePath()));
    _lbCurImage->setText(annoFile->imageInfo()->imagePath().filePath());

    QImage img = GlobalImageLoader::instance()->loadImage(fileName, frame, GlobalImageLoader::LoadLinearRev);
    if (img.isNull()) {
        GlobalLogger::instance()->logError(QString("Could not load image [%1]").arg(fileName.filePath()));
        QMessageBox::information(this, "AnnoTool", tr("Cannot load %1.").arg(fileName.filePath()));
        return;
    }

    newGraphicsScene(&img);
    if(!GlobalProjectManager::instance()->filterMan()->isEnabled()) {
        loadGraphicsAnnoRaw();
    }
    setToolEnabled(true);
    ui.actionCopy->setEnabled(false);
}

void AnnoToolMainWindow::onPM_annoSelectChanged(int row, QUuid annoId,
        ::anno::dt::Annotation *anno) {
    GlobalLogger::instance()->logDebug(QString("Selected annotation [%1]").arg(row));
    _graphicsScene->selectShape(annoId);
    ui.annoDataWidget->updateAllData();

    if(row >= 0) {
        ui.actionCopy->setEnabled(true);
    } else {
        ui.actionCopy->setEnabled(false);
    }
}

void AnnoToolMainWindow::onFM_filterEnable(bool commonState, bool scoreState) {
    if(!commonState && !scoreState) {
        _graphicsScene->removeAllAnnoShapes();
        loadGraphicsAnnoRaw();
    }
}

void AnnoToolMainWindow::onFM_curFilterBegin(int preAnnoCount) {
    GlobalLogger::instance()->logDebug(QString("Begin annotation filtering of %1 annotations.").arg(preAnnoCount));
}

void AnnoToolMainWindow::onFM_curFilterEnd(int preAnnoCount, int postAnnoCount) {
    GlobalLogger::instance()->logDebug(QString("Finished annotation filtering, filtered %1 out of %2 annotations.").arg(postAnnoCount).arg(preAnnoCount));

    if(GlobalProjectManager::instance()->filterMan() != NULL && GlobalProjectManager::instance()->filterMan()->isEnabled() && _graphicsScene != NULL) {
        _graphicsScene->removeAllAnnoShapes();
        if(postAnnoCount > 0) {
            loadGraphicsAnnoFiltered();
        }
    }
}

void AnnoToolMainWindow::onCM_dataAvailable() {
    if(_copyMan.hasAnnotation()) {
        ui.actionPaste->setEnabled(true);
    }
}

void AnnoToolMainWindow::onCM_lostData() {
    if(!_copyMan.hasAnnotation()) {
        ui.actionPaste->setEnabled(false);
    }
}

void AnnoToolMainWindow::on_actionLockParentAnno_triggered() {
    if(ui.actionLockParentAnno->isChecked()) {
        lockParentAnno(true);
    } else {
        lockParentAnno(false);
    }
}

void AnnoToolMainWindow::on_actionToolPointer_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionToolPointer_triggered");
    GlobalToolManager::instance()->selectTool(GlobalToolManager::GtPointer);
}

void AnnoToolMainWindow::on_actionToolSinglePoint_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionToolSinglePoint_triggered");
    GlobalToolManager::instance()->selectTool(GlobalToolManager::GtSinglePoint);
}

void AnnoToolMainWindow::on_actionToolRectangle_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionToolRectangle_triggered");
    GlobalToolManager::instance()->selectTool(GlobalToolManager::GtRect);
}

void AnnoToolMainWindow::on_actionToolPolygon_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionToolPolygon_triggered");
    GlobalToolManager::instance()->selectTool(GlobalToolManager::GtPolygon);
}

void AnnoToolMainWindow::on_actionToolEllipse_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionToolEllipse_triggered");
    GlobalToolManager::instance()->selectTool(GlobalToolManager::GtEllipse);
}

void AnnoToolMainWindow::onTM_toolSelected(anno::GlobalToolManager::SelGraphicsTool tool, bool reset) {
    if (!reset) {
        switch (tool) {
            case anno::GlobalToolManager::GtNone2:
            case anno::GlobalToolManager::GtNone: {
                    uncheckTools();
                    break;
                }
            case anno::GlobalToolManager::GtPointer: {
                    uncheckTools();
                    ui.actionToolPointer->setChecked(true);
                    break;
                }
//			case anno::GlobalToolManager::GtHand:
//			{
//				uncheckTools();
//				ui.actionToolHand->setChecked(true);
//				break;
//			}
            case anno::GlobalToolManager::GtRect: {
                    uncheckTools();
                    ui.actionToolRectangle->setChecked(true);
                    break;
                }
            case anno::GlobalToolManager::GtPolygon: {
                    uncheckTools();
                    ui.actionToolPolygon->setChecked(true);
                    break;
                }
            case anno::GlobalToolManager::GtSinglePoint: {
                    uncheckTools();
                    ui.actionToolSinglePoint->setChecked(true);
                    break;
                }
            case anno::GlobalToolManager::GtEllipse: {
                    uncheckTools();
                    ui.actionToolEllipse->setChecked(true);
                    break;
                }
            default:
                GlobalLogger::instance()->logWarning(
                    "MW: Aborted tool selection state change due to unknown tool type.");
        }
    }
}

void AnnoToolMainWindow::updateUI() {
    if (_me != NULL) {
        _me->updateAnnoWidgets();
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
