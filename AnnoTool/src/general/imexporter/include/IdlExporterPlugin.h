#ifndef IDLEXPORTERPLUGIN_H_
#define IDLEXPORTERPLUGIN_H_

#include "ExporterPlugin.h"
#include "AnnoRectangle.h"
#include "libAn_AnnoRect.h"

namespace anno {

    class IdlExporterPlugin : public anno::ExporterPlugin {
        private:
            libAn::AnnoRect convRect(dt::AnnoRectangle *rect);

        public:
            IdlExporterPlugin();
            virtual ~IdlExporterPlugin();

        public:
            virtual QString name() const;
            virtual QString author() const;
            virtual QString description() const;
            virtual bool exec(const QFileInfo &startDir);
    };

}

#endif /*IDLEXPORTERPLUGIN_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
