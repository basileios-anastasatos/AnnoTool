#ifndef GLOBALIMAGELOADER_H_
#define GLOBALIMAGELOADER_H_

#include <QImage>
#include <QPair>
#include "ImageLoaderPlugin.h"

#ifndef NOFRAME
#define NOFRAME -1
#endif

namespace anno {
    /**
     * This class is an default implementation of the ImageLoaderPlugin interface
     * for Qt's native image types.
     * This default plugin implementation handles the loading of Qt's natively
     * supported image types such as JPG, PNG, TIFF ...<br><br>
     * Note that the list of natively supported image types and formats can vary
     * from system to system and is depending on Qt's compilation configuration.
     *
     * @author Oliver Schwahn
     */
    class DefaultImageLoaderPlugin : public ImageLoaderPlugin {
        public:
            /**
             * Default constructor.
             */
            DefaultImageLoaderPlugin() {
            }

            /**
             * Virtual destructor.
             */
            virtual ~DefaultImageLoaderPlugin() {
            }

        public:
            virtual QString author() const;
            virtual QString name() const;
            virtual QString description() const;
            virtual QList<QString> imageTypes() const;
            virtual QList<QString> fileTypes() const;
            virtual bool canLoad(const QFileInfo &file) const;
            virtual QImage loadImage(const QFileInfo &file, int frame) const;
    };

    /**
     * This class is the application wide manager for loading annotation
     * image files.
     * This class is designed as global singleton to enable easy application
     * wide access to the provided image loading functionality. By default,
     * only Qt's native image types can be loaded (this is done by the default
     * plugin). The set of supported image types and formats can easily be extended
     * by implementing the ImageLoaderPlugin interface and adding those implementing
     * classes during runtime to the GlobalImageLoader's plugin set.<br><br>
     * Note that there are several loading strategies supportet, see the loading
     * methods for details.
     *
     * @author Oliver Schwahn
     */
    class GlobalImageLoader {
        private:
            /**
             * The singleton pointer to the only class instance.
             */
            static GlobalImageLoader *_me;

        private:
            /**
             * List of all currently available loader plugins.
             */
            QList<ImageLoaderPlugin *> _pluginList;

            /**
             * The default loader plugin that handles Qt's native image types.
             */
            ImageLoaderPlugin *_defaultPlugin;

        private:
            /**
             * The private singleton constructor.
             */
            GlobalImageLoader();

            /**
             * Internal helper for initializing the singleton.
             */
            static void setupImageLoader();

        public:
            /**
             * Virtual destructor.
             */
            virtual ~GlobalImageLoader();

            /**
             * Singleton access method.
             * This method enables the global access to the only class instance.
             *
             * @return The only class instance.
             */
            static GlobalImageLoader *instance();

            /**
             * Singleton reset method.
             * This method provides the capability to reset the only existing
             * class instance.
             */
            static void reset();

        public:
            enum LoadStrategy {
                LoadDefault, LoadExplicit, LoadLinear, LoadLinearRev
            };

        public:
            static bool fileTypeLoadable(const QString &ext, ImageLoaderPlugin *plugin);
            bool canLoad(const QString &filePath) const;
            bool canLoad(const QFileInfo &file) const;

            bool canLoadNative(const QString &filePath) const;
            bool canLoadNative(const QFileInfo &file) const;

            QImage loadImage(const QString &filePath, LoadStrategy s = LoadLinear) const;
            QImage loadImage(const QFileInfo &file, LoadStrategy s = LoadLinear) const;
            QImage
            loadImage(const QString &filePath, int frame, LoadStrategy s = LoadLinear) const;
            QImage
            loadImage(const QFileInfo &file, int frame, LoadStrategy s = LoadLinear) const;

            QImage loadImage(const QString &filePath, int plugin) const;
            QImage loadImage(const QFileInfo &file, int plugin) const;
            QImage loadImage(const QString &filePath, int frame, int plugin) const;
            QImage loadImage(const QFileInfo &file, int frame, int plugin) const;

        public:
            void addPlugin(ImageLoaderPlugin *plugin);
            void removePlugin(int plugin);
            ImageLoaderPlugin *getPlugin(int plugin);
            int pluginCount();
            QList< QPair<QString, int> > mergedImageTypes() const;
            QList< QPair<QString, int> > mergedFileTypes() const;

    };

}

#endif /*GLOBALIMAGELOADER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
