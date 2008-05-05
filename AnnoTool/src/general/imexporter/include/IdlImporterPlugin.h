#ifndef IDLIMPORTERPLUGIN_H_
#define IDLIMPORTERPLUGIN_H_

#include "ImporterPlugin.h"

namespace anno {

    class IdlImporterPlugin : public anno::ImporterPlugin {
        public:
            IdlImporterPlugin();
            virtual ~IdlImporterPlugin();

        public:
            virtual QString name() const;
            virtual QString author() const;
            virtual QString description() const;
            virtual bool singleFileImport() const;
            virtual bool singleAnnotationImport() const;
            virtual bool exec(const QFileInfo &startDir);
    };

}

#endif /*IDLIMPORTERPLUGIN_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
