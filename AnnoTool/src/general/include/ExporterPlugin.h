#ifndef EXPORTERPLUGIN_H_
#define EXPORTERPLUGIN_H_

#include <QString>
#include <QFileInfo>

namespace anno {
    class ExporterPlugin {
        protected:
            ExporterPlugin() {
            }

        public:
            virtual ~ExporterPlugin() {
            }

        public:
            virtual QString name() const = 0;
            virtual QString author() const = 0;
            virtual QString description() const = 0;
            virtual bool exec(const QFileInfo &startDir, bool silent = false) = 0;

    };
}

#endif /*EXPORTERPLUGIN_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
