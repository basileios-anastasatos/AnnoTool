#include "include/IdlExporterPlugin.h"
#include "importGlobals.h"
#include "libAn_AnnotationList.h"
#include "AnnoRectangle.h"
#include "DlgIdlExporter.h"

#include <QFileInfo>
#include <QFileDialog>

namespace anno {

    IdlExporterPlugin::IdlExporterPlugin() :
        _rectangle(true), _ellipse(false), _polygon(false), _singlePoint(false), _relativPaths(true), _idlDir(".") {
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
        return QString("Exports to the old IDL immage annotation format.");
    }

    bool IdlExporterPlugin::exec(const QFileInfo &startDir) {
        QString idlPath = QFileDialog::getSaveFileName(NULL,
                          "Select a location where to export", startDir.filePath());
        QFileInfo idlFile(idlPath);
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
            delete dlg;
        } else {
            delete dlg;
            GlobalLogger::instance()->logWarning("User canceled exporter!");
            return false;
        }
        _idlDir = idlFile.absoluteDir();

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
                    anno.addAnnoRect(r);
                    ++annoCount;
                }
            }
            annoList.addAnnotation(anno);
            ++imgCount;
        }

        annoList.saveIDL(idlPath.toStdString());
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
                return _singlePoint;
            case dt::ASTypeRectangle:
                return _rectangle;
            case dt::ASTypePolygon:
                return _polygon;
            case dt::ASTypeEllipse:
                return _ellipse;
            default:
                return false;
        }
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
