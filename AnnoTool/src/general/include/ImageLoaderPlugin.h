#ifndef IMAGELOADERPLUGIN_H_
#define IMAGELOADERPLUGIN_H_

#include <QImage>
#include <QFileInfo>
#include <QString>
#include <QList>

namespace anno {

    /**
     * Abstract Interface for extending the GlobalImageLoader's capabilities.
     * By implementing this interface support for additional image types can be
     * added to the GlobalImageLoader, such that new image files/types can be
     * loaded and worked with by AnnoTool.<br>
     * This interface is part of an abstraction layer that handles AnnoTool's
     * image loading. This layer was introduced so that the Qt native image IO
     * plugin mechanism (involving QImageReader, QImageIOPlugin, QImageIOHandler)
     * must not be used to enable an easier image loading approach.<br><br>
     * <strong>Note that all pure virtual methods should be implemented
     * according to their detailed description! All implementing classes
     * of this interface MUST NOT throw any exceptions from within the abstract
     * interface methods.</strong>
     *
     * @author Oliver Schwahn
     */
    class ImageLoaderPlugin {
        protected:
            /**
             * Default constructor.
             * This constructor is protected to demonstrate that
             * this is an abstract interface class.
             */
            ImageLoaderPlugin() {
            }

        public:
            /**
             * Virtual dummy destructor.
             * This virtual destructor ensures that all child class
             * destructors are virtual too.
             */
            virtual ~ImageLoaderPlugin() {
            }

            // Actual public interface
            //-------------------------------------------------------------------
        public:
            /**
             * This method is used to determine the implementing class' author.
             * The information provided by this method is not vital, it is only
             * used as user information and should therefore be implemented
             * reasonably.
             *
             * @return The author's name of the implementing class.
             */
            virtual QString author() const = 0;

            /**
             * This method is used to retrieve the plugin name.
             * The information provided by this method is not vital, it is only
             * used as user information and should therefore be implemented
             * reasonably.
             *
             * @return The plugin's name.
             */
            virtual QString name() const = 0;

            /**
             * This method is used to retrieve a short plugin description.
             * The information provided by this method is not vital, it is only
             * used as user information and should therefore be implemented
             * reasonably.
             *
             * @return A short plugin description.
             */
            virtual QString description() const = 0;

            /**
             * This method is used to retrieve a list of image types that
             * can be loaded by the implementing class.
             * The GlobalImageLoader invokes this method to retrieve a list
             * of all image types that are supported by the implementing class.
             * This information is used to inform the user about the plugin's
             * capabilities. Correct information about the supported types is not
             * vital to the operability of the system but maybe for the user
             * who operates it. The returned list can be empty if no type information
             * is provided.<br><br>
             * <i>Sample Data</i><br>
             * <ul>
             * <li>JPEG - Standard JPEG-Files</li>
             * <li>PNG - Standard PNG-Files</li>
             * </ul>
             *
             * @return A list of all supported image types.
             */
            virtual QList<QString> imageTypes() const = 0;

            /**
             * This method is used to retrieve a list of all image file extensions
             * that can be processed by the implementing class.
             * The GlobalImageLoader invokes this method to retrieve a list
             * of all image file extensions that are supported by the implementing class.
             * This information is used to inform the user about the plugin's
             * capabilities. Furthermore this information is used to filter upcoming
             * image files such that just the ones with compatible file extensions are
             * passed to the implementing class for probing/loading.<br>
             * Correct information about the supported file extensions is <strong>vital
             * to the operability of the system</strong>. The returned list can be empty
             * if all upcoming image files - independent of the file extension - should
             * be passed to the implementing class for probing/loading.<br><br>
             * <i>Sample Data</i><br>
             * <ul>
             * <li>jpg</li>
             * <li>jpeg</li>
             * <li>png</li>
             * <li>gif</li>
             * </ul>
             * Note that the file extensions must be given without any wildcards or
             * separators like '*', '.' and the like.
             *
             * @return A list of all supported image file extensions.
             */
            virtual QList<QString> fileTypes() const = 0;

            /**
             * This method is used for probing whether a given file can be loaded
             * using the implementing class.
             * The GlobalImageLoader invokes this method to determine whether the
             * implementing class can be used for loading the given image file.
             * This method is invoked only for upcoming image files that has one
             * of the file extensions retrieved from fileTypes().
             *
             * @param file File to be probed.
             *
             * @return True if the given file can be loaded using the implementing
             * class, false otherwise.
             */
            virtual bool canLoad(const QFileInfo &file) const = 0;

            /**
             * This method is used for the actual image loading.
             * The GlobalImageLoader invokes this method to load a given image.
             * This method is only invoked for upcoming image files which were
             * identified as loadable by canLoad(const QFileInfo&) of the
             * implementing class.<br>
             * The QImage returned by this method must be an independent QImage
             * instance such that it can be fully managed outside the creating
             * loader plugin without overhead (beware of implicit data sharing).
             * If the given image file could not be loaded due to whatever
             * reasons this method must return a so-called Null-Image (see QImage()).
             *
             * @param file File to be loaded.
             *
             * @return The loaded image or a Null-Image.
             */
            virtual QImage loadImage(const QFileInfo &file) const = 0;
            //-------------------------------------------------------------------
    };

}

#endif /*IMAGELOADERPLUGIN_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
