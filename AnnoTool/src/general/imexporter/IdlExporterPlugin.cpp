#include "include/IdlExporterPlugin.h"
#include "importGlobals.h"
#include "libAn_AnnotationList.h"
#include "AnnoRectangle.h"
#include "AnnoSinglePoint.h"
#include "DlgIdlExporter.h"

#include <QFileInfo>
#include <QFileDialog>

namespace anno {

    IdlExporterPlugin::IdlExporterPlugin() :
        _rectangle(true), _ellipse(false), _polygon(false), _singlePoint(false), _relativPaths(true), _filterRule(NULL), _idlDir(".") {
    }

    IdlExporterPlugin::~IdlExporterPlugin() {
    }

    QString IdlExporterPlugin::name() const {
        return QString("Default IDL Exporter");
    }

    QString IdlExporterPlugin::author() const {
        return QString("Oliver Schwahn");
    }

    QString IdlExporterPlugin::description() const {
        return QString("Exports to the old IDL image annotation format.");
    }

    bool IdlExporterPlugin::exec(const QFileInfo &startDir, bool silent) {
        QString idlPath;
        QFileInfo idlFile;

        if (!silent) {
            idlPath = QFileDialog::getSaveFileName(NULL, "Select a location where to export", startDir.filePath());

            //QFileInfo idlFile(idlPath);
            idlFile.setFile(idlPath);

            if (idlPath.isEmpty()) {
                GlobalLogger::instance()->logWarning("User canceled exporter!");
                return false;
            }

            DlgIdlExporter *dlg = new DlgIdlExporter();
            if(dlg->exec() == QDialog::Accepted) {
                _rectangle = dlg->expRectangles();
                _ellipse = dlg->expEllipses();
                _polygon = dlg->expPolygons();
                _singlePoint = dlg->expSinglePoints();
                _relativPaths = dlg->pathsRelative();
                if(dlg->hasFilter() && dlg->selectedFilter()->hasRule()) {
                    _filterRule = dlg->selectedFilter()->getFilterRule();
                }
                delete dlg;
            } else {
                delete dlg;
                GlobalLogger::instance()->logWarning("User canceled exporter!");
                return false;
            }
        } else {
            /* MA: some defaults to use when silently exporting the project */
            idlFile = startDir;
            idlPath = idlFile.absoluteFilePath();

            _rectangle = true;
            _ellipse = false;
            _polygon = false;
            _singlePoint = false;
            _relativPaths = false;
        }

        _idlDir = idlFile.absoluteDir();

        /* MA, save in al/idl depending on the extension */
        if (!idlPath.endsWith(".al", Qt::CaseInsensitive) &&
                !idlPath.endsWith(".idl", Qt::CaseInsensitive)) {
            std::cout << "unsupported format, idlPath: " << idlPath.toStdString() << std::endl;
            assert(false && "unsupported format");
        }

        int imgCount = 0;
        int annoCount = 0;
        libAn::AnnotationList annoList;
        GlobalProjectManager *pm = GlobalProjectManager::instance();
        QListIterator<dt::AnnoFileData *> iFile = pm->getFileIterator();
        while (iFile.hasNext()) {
            dt::AnnoFileData *curFile = iFile.next();
            libAn::Annotation anno;
            anno.setImageName(processImagePath(curFile->imageInfo()->imagePath()).toStdString());
            if (curFile->imageInfo()->frame() != NOFRAME) {
                anno.setFrameNr(curFile->imageInfo()->frame());
            }

            QListIterator<dt::Annotation *> iShape = curFile->getAnnoIterator();
            while (iShape.hasNext()) {
                dt::Annotation *curShape = iShape.next();
                if(isExportShape(curShape)) {
                    libAn::AnnoRect r = convRect(curShape);
                    if (curShape->hasScore()) {
                        r.setScore(curShape->score());
                    }

                    /* MA BEGIN: add annopoints */
                    QList<QUuid> children_list = curShape->annoChildren();

                    QListIterator<QUuid> it(children_list);

                    while (it.hasNext()) {
                        QUuid childUuid = it.next();
                        Annotation *childAnno = curFile->getAnnotation(childUuid);
                        assert(childAnno != 0);

                        QString qsId;
                        if (childAnno->getClassAttributeValue(NATIVE_CLASS_POSEPOINT,
                                                              NATIVE_POSEPOINT_ID_ATTR,
                                                              qsId)) {
                            bool ok = false;
                            int ap_id = qsId.toInt(&ok);
                            assert(ok);

                            dt::AnnoShape *childShape = childAnno->shape();
                            assert(childShape->shapeType() == anno::dt::ASTypeSinglePoint);
                            dt::AnnoSinglePoint *childSinglePoint = (dt::AnnoSinglePoint *)childShape;

                            libAn::AnnoPoint ap;
                            ap.x = (int)(childSinglePoint->x());
                            ap.y = (int)(childSinglePoint->y());
                            ap.id = ap_id;

                            r.m_vAnnoPoints.push_back(ap);
                        }

                    }// children

                    // sort annopoints by id ...
                    std::sort(r.m_vAnnoPoints.begin(),
                              r.m_vAnnoPoints.end(),
                              libAn::compAnnoPointById());

                    /* MA END */


                    anno.addAnnoRect(r);
                    ++annoCount;

                }
            }
            annoList.addAnnotation(anno);
            ++imgCount;
        }

        annoList.save(idlPath.toStdString());

        //annoList.saveIDL(idlPath.toStdString());
        GlobalLogger::instance()->logInfo(QString("Exported %1 images and %2 annotations to IDL.").arg(imgCount).arg(annoCount));
        return true;
    }

    libAn::AnnoRect IdlExporterPlugin::convRect(dt::Annotation *anno) {
        libAn::AnnoRect arect;
        qreal x1 = 0.0;
        qreal y1 = 0.0;
        qreal x2 = 0.0;
        qreal y2 = 0.0;
        QRectF boundingRect = anno->shape()->boundingRect();
        boundingRect.getCoords(&x1, &y1, &x2, &y2);
        arect.setCoords(qRound(x1), qRound(y1), qRound(x2), qRound(y2));
        return arect;
    }

    bool IdlExporterPlugin::isExportShape(dt::Annotation *anno) {
        if(anno == NULL || anno->shape() == NULL) {
            return false;
        }

        dt::AnnoShapeType type = anno->shape()->shapeType();
        switch(type) {
            case dt::ASTypeSinglePoint:
                return checkExport(_singlePoint, anno);
            case dt::ASTypeRectangle:
                return checkExport(_rectangle, anno);
            case dt::ASTypePolygon:
                return checkExport(_polygon, anno);
            case dt::ASTypeEllipse:
                return checkExport(_ellipse, anno);
            default:
                return false;
        }
    }

    bool IdlExporterPlugin::checkExport(bool shapeType, dt::Annotation *anno) const {
        if (shapeType) {
            if (_filterRule != NULL) {
                return _filterRule->eval(anno);
            }
            return true;
        }
        return false;
    }

    QString IdlExporterPlugin::processImagePath(const QFileInfo &imgPath) const {
        QString absPath;
        if(!imgPath.isAbsolute()) {
            absPath = GlobalProjectManager::instance()->relToAbs(imgPath).canonicalFilePath();
        } else {
            absPath = imgPath.canonicalFilePath();
        }

        return _relativPaths ? _idlDir.relativeFilePath(absPath) : absPath;
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
