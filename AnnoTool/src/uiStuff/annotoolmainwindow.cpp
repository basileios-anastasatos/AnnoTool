#include "include/annotoolmainwindow.h"
#include "include/zoomcontrol.h"
#include "include/dlgnewproject.h"
#include "include/DlgProjectDetails.h"
#include "include/DlgAddImage.h"
#include "include/DlgLoaderDetails.h"
#include "AllAnnoExceptions.h"
#include "AnnoFileData.h"
#include "GlobalLogger.h"
#include "GlobalConfig.h"
#include "GlobalProjectManager.h"
#include "GlobalImageLoader.h"
#include "AnnoGraphicsShapeCreator.h"

#include "../annoGraphics/include/AnnoGraphicsScene.h"
#include "../annoGraphics/include/AnnoGraphicsPixmap.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QImage>
#include <QCloseEvent>

using ::logging::GlobalLogger;
using ::anno::GlobalProjectManager;
using ::anno::GlobalImageLoader;
using namespace ::anno::exc;

AnnoToolMainWindow::AnnoToolMainWindow(QWidget *parent) :
    QMainWindow(parent) {
    _graphicsScene = NULL;
    ui.setupUi(this);
    setCentralWidget(ui.graphicsView);
    ZoomControl *z = new ZoomControl(ui.tbView);
    QAction *za = ui.tbView->addWidget(z);
    za->setVisible(true);

    connect(ui.annoFileListWidget, SIGNAL(annoFileSelectChanged(int, QUuid)), this, SLOT(annoFileSelectChanged(int, QUuid)));
    connect(z, SIGNAL(zoomChanged(int)), this, SLOT(on_zoomSlider_valueChanged(int)));

    configUIproject(false);
}

AnnoToolMainWindow::~AnnoToolMainWindow() {

}

void AnnoToolMainWindow::clearGraphicsScene() {
    if (_graphicsScene != NULL) {
        ui.graphicsView->setScene(NULL);
        delete _graphicsScene;
        _graphicsScene = NULL;
    }
}

void AnnoToolMainWindow::newGraphicsScene(QImage *img) {
    clearGraphicsScene();
    _graphicsScene = new anno::graphics::AnnoGraphicsScene(ui.graphicsView);
    if (img != NULL) {
        _graphicsScene->setAnnoImage(*img);
    }
    ui.graphicsView->setScene(_graphicsScene);
}

void AnnoToolMainWindow::loadGraphicsAnno() {
    if (_graphicsScene != NULL && GlobalProjectManager::instance()->isValid()) {
        anno::dt::AnnoFileData *curFile = GlobalProjectManager::instance()->selectedFile();
        if (curFile != NULL && !curFile->annoList()->isEmpty()) {
            QListIterator<anno::dt::Annotation *> i(*curFile->annoList());
            int j = 0;
            while (i.hasNext()) {
                anno::dt::Annotation *cur = i.next();
                anno::graphics::AnnoGraphicsShape *s =
                    anno::graphics::AnnoGraphicsShapeCreator::toGraphicsShape(cur);
                if (s != NULL) {
                    _graphicsScene->addAnnoShape(s);
                    ++j;
                }
            }
            GlobalLogger::instance()->logInfo(QString("Loaded %1 annotation shapes.").arg(j));
        }
    }
}

void AnnoToolMainWindow::updateAnnoWidgets() {
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
    ui.actionProjectDetails->setEnabled(open);
    ui.actionProjectAddImage->setEnabled(open);
}

void AnnoToolMainWindow::closeEvent(QCloseEvent *event) {
    if (checkProjectToClose()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void AnnoToolMainWindow::on_actionFileNew_triggered() {
    if (checkProjectToClose()) {
        DlgNewProject *dlg = new DlgNewProject(this);
        if (dlg->exec() == QDialog::Accepted) {
            QMessageBox::information(this, "AnnoTool", "New Project ;-)");
            configUIproject(true);
            updateAnnoWidgets();
        }

        delete dlg;
    }
}

void AnnoToolMainWindow::on_actionFileOpen_triggered() {
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
                GlobalLogger::instance()->logInfo(QString("Loaded project '%1': %2 classes, %3 AnnotationFiles.").arg(pm->project()->projectName()).arg(pm->classes()->classCount()).arg(pm->files()->count()));
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

void AnnoToolMainWindow::on_actionFileExit_triggered() {
    close();
}

void AnnoToolMainWindow::on_actionProjectDetails_triggered() {
    DlgProjectDetails *dlg = new DlgProjectDetails(this);
    dlg->exec();
    delete dlg;
}

void AnnoToolMainWindow::on_actionProjectAddImage_triggered() {
    DlgAddImage *dlg = new DlgAddImage();
    if (dlg->exec() == QDialog::Accepted) {
        QList<QString> images = dlg->images();
        if (!images.isEmpty()) {
            GlobalProjectManager *pm = GlobalProjectManager::instance();
            QFileInfo annoPath(dlg->annoSavePath());
            QFileInfo tmpPath = annoPath;
            if (!pm->project()->containsInSearchPathAdv(tmpPath)) {
                pm->project()->classPath()->append(annoPath);
            }

            bool makeRel = dlg->saveImagesRel();
            QListIterator<QString> i(images);
            while (i.hasNext()) {
                QString img = i.next();
                QFileInfo imgPath(img);
                if (makeRel) {
                    imgPath = pm->absToRel(imgPath);
                }

                QUuid uuid = QUuid::createUuid();
                ::anno::dt::AnnoFileData *fd = new ::anno::dt::AnnoFileData(GlobalProjectManager::defAnnoFileName(uuid));
                fd->annoInfo()->setComplexId(pm->project()->uuid());
                ::anno::dt::AnnoImageInfo *imgInfo = fd->imageInfo();
                imgInfo->setImageId(uuid);
                imgInfo->setImagePath(imgPath);

                pm->files()->append(fd);
            }
            updateAnnoWidgets();
        }
    }
    delete dlg;
}

void AnnoToolMainWindow::on_actionSetImageLoader_triggered() {
    DlgLoaderDetails *dlg = new DlgLoaderDetails(this);
    dlg->exec();
    delete dlg;
}

void AnnoToolMainWindow::on_appClose() {
    GlobalLogger::instance()->logInfo("AnnoTool is shutting down");
    GlobalProjectManager::instance()->clear();
}

void AnnoToolMainWindow::annoFileSelectChanged(int row, QUuid image) {
    GlobalLogger::instance()->logDebug(QString("Selected new annotation file with index %1 and UUID %2").arg(row).arg(image.toString()));
    GlobalProjectManager::instance()->setSelectedFileRow(row);
    ui.annoListWidget->updateData();
    QFileInfo fileName = GlobalProjectManager::instance()->selectedFile()->imageInfo()->imagePath();
    if (fileName.isRelative()) {
        fileName = GlobalProjectManager::instance()->relToAbs(fileName);
    }
    GlobalLogger::instance()->logDebug(fileName.filePath());
    QImage img = GlobalImageLoader::instance()->loadImage(fileName, GlobalImageLoader::LoadLinearRev);
    if (img.isNull()) {
        QMessageBox::information(this, "AnnoTool", tr("Cannot load %1.").arg(fileName.filePath()));
        return;
    }

    newGraphicsScene(&img);
    loadGraphicsAnno();
    //	ui.graphicsView->invalidateScene();
}

void AnnoToolMainWindow::on_annoListWidget_annoSelectChanged(int row, QUuid anno) {
    _graphicsScene->selectShape(anno);
}

void AnnoToolMainWindow::on_zoomSlider_valueChanged(int value) {
    qreal f = value / 100.0;
    statusBar()->showMessage(tr("Scale: %1").arg(f), 1000);
    ui.graphicsView->resetMatrix();
    ui.graphicsView->scale(f, f);
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
