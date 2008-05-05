#ifndef IMPORTERPLUGIN_H_
#define IMPORTERPLUGIN_H_

#include <QString>
#include <QFileInfo>

namespace anno {
    class ImporterPlugin {
        protected:
            ImporterPlugin() {
            }

        public:
            virtual ~ImporterPlugin() {
            }

        public:
            virtual QString name() const = 0;
            virtual QString author() const = 0;
            virtual QString description() const = 0;
            virtual bool singleFileImport() const = 0;
            virtual bool singleAnnotationImport() const = 0;
            virtual bool exec(const QFileInfo &startDir) = 0;
    };
}

#endif /*IMPORTERPLUGIN_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
