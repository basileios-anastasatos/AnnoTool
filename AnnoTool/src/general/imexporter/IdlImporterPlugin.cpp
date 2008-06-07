#include "include/IdlImporterPlugin.h"
#include "importGlobals.h"
#include <QFileDialog>

#include "libAn_AnnotationList.h"

namespace anno {

    IdlImporterPlugin::IdlImporterPlugin() {
    }

    IdlImporterPlugin::~IdlImporterPlugin() {
    }

    QString IdlImporterPlugin::name() const {
        return QString("Default IDL Importer");
    }

    QString IdlImporterPlugin::author() const {
        return QString("Oliver Schwahn");
    }

    QString IdlImporterPlugin::description() const {
        return QString();
    }

    bool IdlImporterPlugin::singleFileImport() const {
        return false;
    }

    bool IdlImporterPlugin::singleAnnotationImport() const {
        return false;
    }

    bool IdlImporterPlugin::exec(const QFileInfo &startDir) {
        QString idlPath = QFileDialog::getOpenFileName(NULL, "Select an IDL file to import", startDir.filePath());
        QFileInfo idlFile(idlPath);
        if(idlPath.isEmpty() || !idlFile.exists()) {
            return false;
        }

        GlobalLogger::instance()->logInfo(QString("Loading IDL annotation datasets from [%1].").arg(idlPath));
        libAn::AnnotationList annoList;
        annoList.load(idlPath.toStdString());
        GlobalLogger::instance()->logInfo(QString("Loaded %1 IDL annotation datasets.").arg(annoList.size()));

        GlobalProjectManager *pm = GlobalProjectManager::instance();
        QUuid projectUuid = pm->project()->uuid();
        QFileInfo savePath = pm->project()->searchPath()->at(0);
        if(!savePath.isAbsolute()) {
            savePath = pm->relToAbs(savePath);
        }
        //QList<dt::AnnoFileData*>* files = pm->files();

        for(unsigned int i = 0; i < annoList.size(); ++i) {
            libAn::Annotation &curAnno = annoList[i];
            QUuid imgUuid = QUuid::createUuid();
            dt::AnnoFileData *curFileData = new dt::AnnoFileData(savePath.filePath() + "/annotations_" + imgUuid.toString() + ".ata");
            curFileData->annoInfo()->setComplexId(projectUuid);
            curFileData->imageInfo()->setImageId(imgUuid);
            curFileData->imageInfo()->setImagePath(QString(curAnno.imageName().c_str()));
            if(curAnno.isStream()) {
                curFileData->imageInfo()->setFrame(curAnno.frameNr());
            }

            GlobalLogger::instance()->logDebug(QString("Processing annotations for image [%1].").arg(curFileData->imageInfo()->imagePath().filePath()));

            for(unsigned int j = 0; j < curAnno.size(); ++j) {
                libAn::AnnoRect &curRect = curAnno[j];
                dt::Annotation *newAnno = new dt::Annotation();
                newAnno->setAnnoId(QUuid::createUuid());
                newAnno->setShape(convRect(curRect));
                newAnno->addAttribute(dt::AnnoAttribute("idlscore", QString(), QString::number(curRect.score(), 'f', 6)));
                curFileData->addAnnotation(newAnno);
            }
            pm->addAnnoFile(curFileData);
        }

        return true;
    }

    dt::AnnoRectangle *IdlImporterPlugin::convRect(libAn::AnnoRect &rect) {
        dt::AnnoRectangle *arect = new dt::AnnoRectangle();
        arect->setCoords(rect.x1(), rect.y1(), rect.x2(), rect.y2());
        return arect;
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
