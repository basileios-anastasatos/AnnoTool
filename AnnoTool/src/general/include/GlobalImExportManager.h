#ifndef GLOBALIMEXPORTMANAGER_H_
#define GLOBALIMEXPORTMANAGER_H_

#include "ImporterPlugin.h"
#include "ExporterPlugin.h"
#include <QMutex>

namespace anno {

    class GlobalImExportManager {
        private:
            /**
             * The singleton pointer to the only class instance.
             */
            static GlobalImExportManager *_me;

            // other private members
        private:
            /**
             * Mutex for thread safety.
             */
            mutable QMutex _mutex;
            /**
             * List of all currently available annotation data importer plugins.
             */
            QList<ImporterPlugin *> _importer;
            /**
             * List of all currently available annotation data exporter plugins.
             */
            QList<ExporterPlugin *> _exporter;

            // private singleton stuff
        private:
            /**
             * The private singleton constructor.
             */
            GlobalImExportManager();

            /**
             * Internal helper for initializing the singleton.
             */
            static void setupGlobalImExportManager();

            // destructor
        public:
            /**
             * Virtual destructor.
             */
            virtual ~GlobalImExportManager();

            // public singleton stuff
        public:
            /**
             * Singleton access method.
             * This method enables the global access to the only class instance.
             *
             * @return The only class instance.
             */
            static GlobalImExportManager *instance();

            /**
             * Singleton reset method.
             * This method provides the capability to reset the only existing
             * class instance.
             */
            static void reset();

            // general public interface
        public:
            int importerCount() const;
            int exporterCount() const;

            void addImporter(ImporterPlugin *importer);
            void addExporter(ExporterPlugin *exporter);

            void insertImporter(int index, ImporterPlugin *importer);
            void insertExporter(int index, ExporterPlugin *exporter);

            ImporterPlugin *getImporter(int index) const;
            ExporterPlugin *getExporter(int index) const;

            void removeImporter(int index);
            void removeExporter(int index);
            void removeAllImporter();
            void removeAllExporter();

            void deleteImporter(int index);
            void deleteExporter(int index);
            void deleteAllImporter();
            void deleteAllExporter();

    };

}

#endif /*GLOBALIMEXPORTMANAGER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
