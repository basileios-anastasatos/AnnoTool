#ifndef IMPORTERPLUGIN_H_
#define IMPORTERPLUGIN_H_

namespace anno {
    class ImporterPlugin {
        protected:
            ImporterPlugin();

        public:
            ~ImporterPlugin();

        public:
            virtual QString name() const;
            virtual QString author() const;
            virtual QString description() const;
            virtual bool createsProject() const;
            virtual bool needsProject() const;
            virtual bool singleFileImport() const;
            virtual bool singleAnnotationImport() const;
            virtual bool exec(const QFileInfo &startDir);
    };
}

#endif /*IMPORTERPLUGIN_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
