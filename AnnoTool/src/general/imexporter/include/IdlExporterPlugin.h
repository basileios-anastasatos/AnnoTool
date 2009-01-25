#ifndef IDLEXPORTERPLUGIN_H_
#define IDLEXPORTERPLUGIN_H_

#include "ExporterPlugin.h"
#include "Annotation.h"
#include "libAn_AnnoRect.h"
#include "annoFiltersForward.h"

#include <QDir>

namespace anno {

    class IdlExporterPlugin : public anno::ExporterPlugin {
        private:
            bool _rectangle;
            bool _ellipse;
            bool _polygon;
            bool _singlePoint;
            bool _relativPaths;
            filter::AnnoFilterRule *_filterRule;

            QDir _idlDir;

        private:
            libAn::AnnoRect convRect(dt::Annotation *anno);
            bool isExportShape(dt::Annotation *anno);
            QString processImagePath(const QFileInfo &imgPath) const;
            bool checkExport(bool shapeType, dt::Annotation *anno) const;

        public:
            IdlExporterPlugin();
            virtual ~IdlExporterPlugin();

        public:
            virtual QString name() const;
            virtual QString author() const;
            virtual QString description() const;
            virtual bool exec(const QFileInfo &startDir, bool silent = false);
    };

}

#endif /*IDLEXPORTERPLUGIN_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
