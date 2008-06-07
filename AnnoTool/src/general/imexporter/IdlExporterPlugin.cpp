#include "include/IdlExporterPlugin.h"
#include "importGlobals.h"
#include <QFileDialog>

#include "libAn_AnnotationList.h"

namespace anno {

    IdlExporterPlugin::IdlExporterPlugin() {
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
        return QString();
    }

    bool IdlExporterPlugin::exec(const QFileInfo &startDir) {
        QString idlPath = QFileDialog::getSaveFileName(NULL,
                          "Select a location where to export", startDir.filePath());
        QFileInfo idlFile(idlPath);
        if (idlPath.isEmpty()) {
            return false;
        }

        libAn::AnnotationList annoList;
        GlobalProjectManager *pm = GlobalProjectManager::instance();
        QListIterator<dt::AnnoFileData *> iFile = pm->getFileIterator();
        while (iFile.hasNext()) {
            dt::AnnoFileData *curFile = iFile.next();
            libAn::Annotation anno;
            anno.setImageName(curFile->imageInfo()->imagePath().filePath().toStdString());
            if (curFile->imageInfo()->frame() != NOFRAME) {
                anno.setFrameNr(curFile->imageInfo()->frame());
            }

            QListIterator<dt::Annotation *> iShape = curFile->getAnnoIterator();
            while (iShape.hasNext()) {
                dt::Annotation *curShape = iShape.next();
                libAn::AnnoRect r = convRect(reinterpret_cast<dt::AnnoRectangle *>(curShape));
                //TODO score berücksichtigen
                //r.setScore();
                anno.addAnnoRect(r);
            }
            annoList.addAnnotation(anno);
        }

        annoList.saveIDL(idlPath.toStdString());
        return true;
    }

    libAn::AnnoRect IdlExporterPlugin::convRect(dt::AnnoRectangle *rect) {
        //TODO fix here!
        libAn::AnnoRect arect;
        qreal x1, y1, x2, y2;
        rect->getCoords(&x1, &y1, &x2, &y2);
        arect.setCoords((int)(x1), static_cast<int>(y1), static_cast<int>(x2),
                        static_cast<int>(y2));
        return arect;
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
