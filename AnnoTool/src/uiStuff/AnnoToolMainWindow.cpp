#include "include/AnnoToolMainWindow.h"
#include "include/ZoomControl.h"
#include "include/DlgNewProject.h"
#include "include/DlgProjectDetails.h"
#include "include/DlgAddImage.h"
#include "include/DlgLoaderDetails.h"
#include "include/DlgAbout.h"

#include "AllAnnoExceptions.h"
#include "importGlobals.h"
#include "AnnoFileData.h"

#include "AnnoGraphicsScene.h"
#include "AnnoGraphicsPixmap.h"
#include "AnnoGraphicsShapeCreator.h"

#include "DlgImporter.h"
#include "DlgExporter.h"
#include "ImporterPlugin.h"
#include "ExporterPlugin.h"
#include "IdlExporterPlugin.h"

#include <QFileDialog>
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

#ifdef QT_DEBUG
    setWindowTitle(QString("AnnoTool v2 [Debug Version - %1 %2]").arg(__DATE__).arg(__TIME__));
#endif

    setCentralWidget(ui.graphicsView);
    zoomCtrl = new ZoomControl(ui.tbView);
    QAction *za = ui.tbView->addWidget(zoomCtrl);
    za->setVisible(true);

    bool connectOk = true;
    connectOk = connectOk && connect(ui.annoFileListWidget, SIGNAL(annoFileSelectChanged(int, QUuid)), this, SLOT(annoFileSelectChanged(int, QUuid)));
    connectOk = connectOk && connect(zoomCtrl, SIGNAL(zoomChanged(int)), this, SLOT(onZoomCtrl_zoomChanged(int)));
    _me = this;

    GlobalProjectManager *pm = GlobalProjectManager::instance();
    connectOk = connectOk && connect(pm, SIGNAL(curAnnoModified(::anno::dt::Annotation *)), ui.annoDataWidget, SLOT(updateShapeInfo()));
    connectOk = connectOk && connect(pm, SIGNAL(curAnnoFileModifyStateChanged(::anno::dt::AnnoFileData *, bool, bool)), this, SLOT(onPM_fileListUpdate()));
    connectOk = connectOk && connect(pm, SIGNAL(curAnnoModifyStateChanged(::anno::dt::Annotation *, bool, bool)), this, SLOT(onPM_annoListUpdate()));
    connectOk = connectOk && connect(pm, SIGNAL(curAnnoFileSelChanged(int, QUuid, ::anno::dt::AnnoFileData *)), this, SLOT(onPM_annoFileSelectChanged(int, QUuid, ::anno::dt::AnnoFileData *)));
    connectOk = connectOk && connect(pm, SIGNAL(curAnnoSelChanged(int, QUuid, ::anno::dt::Annotation *)), this, SLOT(onPM_annoSelectChanged(int, QUuid, ::anno::dt::Annotation *)));

    if(!connectOk) {
        GlobalLogger::instance()->logError("CONNECT-ERROR: AnnoToolMainWindow::AnnoToolMainWindow(QWidget)");
    }

    configUIproject(false);
    setToolEnabled(false);
}

AnnoToolMainWindow::~AnnoToolMainWindow() {

}

void AnnoToolMainWindow::clearGraphicsScene() {
    GlobalLogger::instance()->logDebug("MW: clearing graphics scene.");
    setToolEnabled(false);
    if (_graphicsScene != NULL) {
        ui.graphicsView->setScene(NULL);
        delete _graphicsScene;
        _graphicsScene = NULL;
    }
    GlobalToolManager::instance()->resetAll();
}

void AnnoToolMainWindow::newGraphicsScene(QImage *img) {
    GlobalLogger::instance()->logDebug("MW: creating new graphics scene.");
    clearGraphicsScene();
    _graphicsScene = new anno::graphics::AnnoGraphicsScene(ui.graphicsView);
    if (img != NULL) {
        _graphicsScene->setAnnoImage(*img);
    }
    ui.graphicsView->setScene(_graphicsScene);
    GlobalToolManager::instance()->setView(ui.graphicsView);
    GlobalToolManager::instance()->setScene(_graphicsScene);
    fitGraphicsScene();
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
        GlobalLogger::instance()->logDebug(QString("MW: fitting scene with faktor: %1 (%2x%3 -> %4x%5)").arg(factor).arg(imgSize.width()).arg(imgSize.height()).arg(vWidth).arg(vHeight));
        zoomCtrl->setZoom(static_cast<int>(factor * 100.0));
    }
}

void AnnoToolMainWindow::loadGraphicsAnno() {
    if (_graphicsScene != NULL && GlobalProjectManager::instance()->isValid()) {
        GlobalLogger::instance()->logDebug("MW: loading annotation shapes into scene.");
        anno::dt::AnnoFileData *curFile = GlobalProjectManager::instance()->selectedFile();
        if (curFile != NULL && curFile->annoCount() != 0) {
            QListIterator<anno::dt::Annotation *> i = curFile->getAnnoIterator();
            int j = 0;
            while (i.hasNext()) {
                anno::dt::Annotation *cur = i.next();
                anno::graphics::AnnoGraphicsShape *s =
                    anno::graphics::AnnoGraphicsShapeCreator::toGraphicsShape(cur);
                if (s != NULL) {
                    GlobalLogger::instance()->logDebug(QString("MW: Adding annotation shape %1").arg(s->relatedAnno()->annoIdAsString()));
                    _graphicsScene->addAnnoShape(s);
                    ++j;
                }
            }
            GlobalLogger::instance()->logInfo(QString("Loaded %1 annotation shapes.").arg(j));
        }
    }
}

void AnnoToolMainWindow::updateAnnoWidgets() {
    GlobalLogger::instance()->logDebug("MW: Updating anno widgets.");
    ui.annoFileListWidget->updateData();
    ui.annoListWidget->updateData();
    //	ui.annoDataWidget->updateData();
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
            GlobalProjectManager::instance()->clear();
            configUIproject(false);
            clearGraphicsScene();
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
    ui.actionToolPointer->setEnabled(enabled);
    ui.actionToolSinglePoint->setEnabled(enabled);
    ui.actionToolRectangle->setEnabled(enabled);
    ui.actionToolPolygon->setEnabled(enabled);
    ui.actionToolEllipse->setEnabled(enabled);

    ui.actionZtoFront->setEnabled(enabled);
    ui.actionRemoveAnnotation->setEnabled(enabled);
    ui.actionSaveCurrentImage->setEnabled(enabled);
}

void AnnoToolMainWindow::closeEvent(QCloseEvent *event) {
    if (checkProjectToClose()) {
        event->accept();
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
            QFileInfo projectPath = dlg->projectPathAsFileInfo();
            pm->newEmpty(projectPath.filePath() + "/" + dlg->projectName() + "."
                         + GlobalConfig::fileExt.projects, projectUuid);
            pm->project()->setProjectName(dlg->projectName());
            pm->project()->addToSearchPath(dlg->defaultPath());
            configUIproject(true);
            updateAnnoWidgets();
        }

        delete dlg;
    }
}

void AnnoToolMainWindow::on_actionFileOpen_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionFileOpen_triggered");

    using ::anno::dt::AnnoFileData;

    if (checkProjectToClose()) {
        QString
        fileName =
            QFileDialog::getOpenFileName(
                this,
                tr("Open AnnoTool Project File"),
                ".",
                :: QString("AnnoTool Project (%1)").arg(anno::FileExtensions::asFilter(::anno::GlobalConfig::fileExt.projects)));
        if (!fileName.isEmpty()) {
            try {
                GlobalProjectManager *pm = GlobalProjectManager::instance();
                pm->loadFromFile(fileName, true);
                GlobalLogger::instance()->logInfo(QString("Loaded project '%1': %2 classes, %3 AnnotationFiles.").arg(pm->project()->projectName()).arg(pm->classCount()).arg(pm->fileCount()));
                configUIproject(true);
                updateAnnoWidgets();
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
            GlobalProjectManager::instance()->clear();
            configUIproject(false);
            clearGraphicsScene();
            updateAnnoWidgets();
        }
    }
}

void AnnoToolMainWindow::on_actionFileSave_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionFileSave_triggered");
    if (GlobalProjectManager::instance()->isValid()) {
        try {
            GlobalProjectManager::instance()->saveToFile(true);
        } catch(AnnoException *e) {
            QMessageBox::critical(this, "AnnoTool Exception", e->getTrace());
            GlobalLogger::instance()->logError(e->getTrace());
            delete e;
        }
    }
}

void AnnoToolMainWindow::on_actionFileImport_triggered() {
    DlgImporter *dlg = new DlgImporter(this);
    if (dlg->exec() == QDialog::Accepted) {
        anno::ImporterPlugin *p = dlg->getSelectedImporterPtr();
        if (p != NULL) {
            if (!p->exec(QFileInfo(GlobalProjectManager::instance()->projectDir(), QString()))) {
                GlobalLogger::instance()->logError(QString("Importer plugin [%1] finished execution with an error.").arg(p->name()));
            } else {
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
            if (!p->exec(QFileInfo(GlobalProjectManager::instance()->projectDir(), QString()))) {
                GlobalLogger::instance()->logError(QString("Exporter plugin [%1] finished execution with an error.").arg(p->name()));
            } else {
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
    DlgProjectDetails *dlg = new DlgProjectDetails(this);
    dlg->exec();
    delete dlg;
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

                pm->addAnnoFile(fd);
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
    GlobalLogger::instance()->logInfo("Writing anno config file.");
    try {
        GlobalConfig::instance()->saveConfig();
    } catch(AnnoException *e) {
        GlobalLogger::instance()->logError(e->msg());
        delete e;
    }
    //TODO reset singletons here!
}

void AnnoToolMainWindow::annoFileSelectChanged(int row, QUuid image) {
    GlobalLogger::instance()->logDebug(QString("Selected new annotation file with index %1 and UUID %2").arg(row).arg(image.toString()));
    GlobalProjectManager::instance()->setSelectedFileRow(row);
}

void AnnoToolMainWindow::on_annoListWidget_annoSelectChanged(int row, QUuid anno) {
    GlobalProjectManager::instance()->setSelectedAnnoRow(row);
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

void AnnoToolMainWindow::on_actionZtoFront_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionZtoFront_triggered");
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if(pm->selectedAnno() != NULL) {
        _graphicsScene->selectShape(pm->selectedAnnoUuid());
        _graphicsScene->bringSelShapeToFront();
    }
}

void AnnoToolMainWindow::on_actionRemoveAnnotation_triggered() {
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    anno::dt::AnnoFileData *curFile = pm->selectedFile();
    if(curFile != NULL) {
        anno::dt::Annotation *curAnno = pm->selectedAnno();
        if(curAnno != NULL) {
            _graphicsScene->removeAnnoShape(curAnno->annoId());
            curFile->removeAnnotation(curAnno->annoId());
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

    ui.annoListWidget->updateData();
    ui.annoDataWidget->updateAllData();

    //TODO handling für sel = -1 !!

    QFileInfo fileName = annoFile->imageInfo()->imagePath();
    int frame = annoFile->imageInfo()->frame();
    if (fileName.isRelative()) {
        fileName = GlobalProjectManager::instance()->relToAbs(fileName);
    }
    GlobalLogger::instance()->logDebug(QString("Loading image [%1]").arg(fileName.filePath()));

    QImage img = GlobalImageLoader::instance()->loadImage(fileName, frame, GlobalImageLoader::LoadLinearRev);
    if (img.isNull()) {
        GlobalLogger::instance()->logError(QString("Could not load image [%1]").arg(fileName.filePath()));
        QMessageBox::information(this, "AnnoTool", tr("Cannot load %1.").arg(fileName.filePath()));
        return;
    }

    newGraphicsScene(&img);
    loadGraphicsAnno();
    //	ui.graphicsView->invalidateScene();
}

void AnnoToolMainWindow::onPM_annoSelectChanged(int row, QUuid annoId,
        ::anno::dt::Annotation *anno) {
    GlobalLogger::instance()->logDebug(QString("Selected annotation [%1]").arg(row));
    _graphicsScene->selectShape(annoId);
    ui.annoDataWidget->updateAllData();
}

void AnnoToolMainWindow::on_actionToolPointer_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionToolPointer_triggered");
    uncheckTools();
    ui.actionToolPointer->setChecked(true);
    GlobalToolManager::instance()->selectTool(GlobalToolManager::GtPointer);
}

void AnnoToolMainWindow::on_actionToolSinglePoint_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionToolSinglePoint_triggered");
    uncheckTools();
    ui.actionToolSinglePoint->setChecked(true);
    GlobalToolManager::instance()->selectTool(GlobalToolManager::GtSinglePoint);
}

void AnnoToolMainWindow::on_actionToolRectangle_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionToolRectangle_triggered");
    uncheckTools();
    ui.actionToolRectangle->setChecked(true);
    GlobalToolManager::instance()->selectTool(GlobalToolManager::GtRect);
}

void AnnoToolMainWindow::on_actionToolPolygon_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionToolPolygon_triggered");
    uncheckTools();
    ui.actionToolPolygon->setChecked(true);
    GlobalToolManager::instance()->selectTool(GlobalToolManager::GtPolygon);
}

void AnnoToolMainWindow::on_actionToolEllipse_triggered() {
    GlobalLogger::instance()->logDebug("MW: actionToolEllipse_triggered");
    uncheckTools();
    ui.actionToolEllipse->setChecked(true);
    GlobalToolManager::instance()->selectTool(GlobalToolManager::GtEllipse);
}

void AnnoToolMainWindow::updateUI() {
    if (_me != NULL) {
        _me->updateAnnoWidgets();
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
