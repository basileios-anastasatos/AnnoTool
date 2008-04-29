#ifndef EXPORTERPLUGIN_H_
#define EXPORTERPLUGIN_H_

namespace anno {
    class ExporterPlugin {
        protected:
            ExporterPlugin();

        public:
            ~ExporterPlugin();

        public:
            virtual QString name() const;
            virtual QString author() const;
            virtual QString description() const;
            virtual bool exec(const QFileInfo &startDir);
    };
}

#endif /*EXPORTERPLUGIN_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
