#ifndef IDLEXPORTERPLUGIN_H_
#define IDLEXPORTERPLUGIN_H_

#include "ExporterPlugin.h"

namespace anno {

    class IdlExporterPlugin : public anno::ExporterPlugin {
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
