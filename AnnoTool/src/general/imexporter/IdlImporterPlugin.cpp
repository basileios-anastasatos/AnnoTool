#include "include/IdlImporterPlugin.h"
#include "importGlobals.h"
#include <QFileDialog>

#include "AnnoPolygon.h"

#include "libAn_AnnotationList.h"

namespace anno {

    QString IdlImporterPlugin::name() const {
        return QString("Default IDL Importer");
    }

    QString IdlImporterPlugin::author() const {
        return QString("Oliver Schwahn");
    }

    QString IdlImporterPlugin::description() const {
        return QString("Imports the old IDL immage annotation format.");
    }

    bool IdlImporterPlugin::singleFileImport() const {
        return false;
    }

    bool IdlImporterPlugin::singleAnnotationImport() const {
        return false;
    }

    bool IdlImporterPlugin::exec(const QFileInfo &startDir, bool silent) {
        QString idlPath;
        QFileInfo idlFile;

        /* MA: */
        if (!silent) {
            QString idlPath = QFileDialog::getOpenFileName(NULL, "Select an IDL file to import", startDir.filePath());
            //QFileInfo idlFile(idlPath);
            idlFile.setFile(idlPath);
        } else {
            idlFile = startDir;
            idlPath = idlFile.absoluteFilePath();
        }

        if(idlPath.isEmpty() || !idlFile.exists()) {
            return false;
        }

        assert((idlPath.endsWith(".al", Qt::CaseInsensitive) ||
                idlPath.endsWith(".idl", Qt::CaseInsensitive)) && "unsupported format");


        GlobalLogger::instance()->logInfo(QString("Loading IDL annotation datasets from [%1].").arg(idlPath));
        libAn::AnnotationList annoList;
        annoList.load(idlPath.toStdString());
        GlobalLogger::instance()->logInfo(QString("Loaded %1 IDL annotation datasets.").arg(annoList.size()));

        GlobalProjectManager *pm = GlobalProjectManager::instance();

        assert(pm->project());

        QUuid projectUuid = pm->project()->uuid();
        assert(!projectUuid.isNull());
        assert(pm->project()->searchPath());

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
                if(curRect.score() != -1.0) {
                    newAnno->setScore(curRect.score());
                }

                curFileData->addAnnotation(newAnno);

                /* MA BEGIN: process annopoints */

                for (unsigned int apidx = 0; apidx < curRect.m_vAnnoPoints.size(); ++apidx) {
                    dt::Annotation *newPosePoint = new dt::Annotation();
                    newPosePoint->setAnnoId(QUuid::createUuid());

                    /* attach shape */
                    dt::AnnoSinglePoint *point_shape = new dt::AnnoSinglePoint();
                    point_shape->setX(curRect.m_vAnnoPoints[apidx].x);
                    point_shape->setY(curRect.m_vAnnoPoints[apidx].y);
                    newPosePoint->setShape(point_shape);

                    /* set class and attributes */
                    newPosePoint->addClass(NATIVE_CLASS_POSEPOINT);

                    anno::dt::AnnoAttribute atr(newPosePoint,
                                                NATIVE_POSEPOINT_ID_ATTR,
                                                NATIVE_CLASS_POSEPOINT,
                                                QString::number(curRect.m_vAnnoPoints[apidx].id));

                    newPosePoint->addAttribute(atr);

                    if (curRect.m_vAnnoPoints[apidx].is_visible != -1) {
                        QString qsIsVis;
                        if (curRect.m_vAnnoPoints[apidx].is_visible == 0) {
                            qsIsVis = "0";
                        } else {
                            qsIsVis = "1";
                        }

                        anno::dt::AnnoAttribute is_vis_atr(newPosePoint,
                                                           NATIVE_POSEPOINT_VISIBLE_ATTR,
                                                           NATIVE_CLASS_POSEPOINT, qsIsVis);
                        newPosePoint->addAttribute(is_vis_atr);
                    }

                    /* set parent/child links */
                    newAnno->addAnnoChild(newPosePoint->annoId());
                    newPosePoint->setAnnoParent(newAnno->annoId());

                    curFileData->addAnnotation(newPosePoint);
                }

                /* MA END */

            }// annorects

            pm->addAnnoFile(curFileData, true);
        }// images

        return true;
    }

    dt::AnnoShape *IdlImporterPlugin::convRect(libAn::AnnoRect &rect) {
        if (rect.x3() > 0) {
            dt::AnnoPolygon *arect = new dt::AnnoPolygon();

            arect->push_back(QPointF(rect.x1(), rect.y1()));
            arect->push_back(QPointF(rect.x2(), rect.y2()));
            arect->push_back(QPointF(rect.x3(), rect.y3()));
            arect->push_back(QPointF(rect.x4(), rect.y4()));

            return arect;
        } else {
            dt::AnnoRectangle *arect = new dt::AnnoRectangle();
            arect->setCoords(rect.x1(), rect.y1(), rect.x2(), rect.y2());
            return arect;
        }
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
