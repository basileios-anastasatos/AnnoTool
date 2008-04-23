#include "include/GlobalImageLoader.h"
//#include <QByteArray>
#include <QImageReader>

namespace anno {
    QString DefaultImageLoaderPlugin::author() const {
        return QString("Oliver Schwahn");
    }

    QString DefaultImageLoaderPlugin::name() const {
        return QString("Default Image Loader");
    }

    QString DefaultImageLoaderPlugin::description() const {
        return QString("Default plugin for loading supported Qt native image types.");
    }

    QList<QString> DefaultImageLoaderPlugin::imageTypes() const {
        QList<QByteArray> lst = QImageReader::supportedImageFormats();
        QList<QString> lstRet;

        if (!lst.isEmpty()) {
            QListIterator<QByteArray> i(lst);
            while (i.hasNext()) {
                lstRet.append(i.next().constData());
            }
        }

        return lstRet;
    }

    QList<QString> DefaultImageLoaderPlugin::fileTypes() const {
        return QList<QString>();
    }

    bool DefaultImageLoaderPlugin::canLoad(const QFileInfo &file) const {
        QImageReader reader(file.filePath());
        return reader.canRead();
    }

    QImage DefaultImageLoaderPlugin::loadImage(const QFileInfo &file) const {
        return QImage(file.filePath());
    }

    GlobalImageLoader *GlobalImageLoader::_me = NULL;

    GlobalImageLoader::GlobalImageLoader() {
        _defaultPlugin = new DefaultImageLoaderPlugin();
    }

    GlobalImageLoader::~GlobalImageLoader() {
        if (_defaultPlugin != NULL) {
            delete _defaultPlugin;
        }
        if (!_pluginList.isEmpty()) {
            QListIterator<ImageLoaderPlugin *> i(_pluginList);
            while (i.hasNext()) {
                delete i.next();
            }
        }
    }

    void GlobalImageLoader::setupImageLoader() {
        _me = new GlobalImageLoader();
    }

    GlobalImageLoader *GlobalImageLoader::instance() {
        if (_me == NULL) {
            setupImageLoader();
        }
        return _me;

    }

    void GlobalImageLoader::reset() {
        if (_me != NULL) {
            delete _me;
            _me = NULL;
        }
    }

    bool GlobalImageLoader::fileTypeLoadable(const QString &ext,
            ImageLoaderPlugin *plugin) {
        if (plugin != NULL) {
            QList<QString> lst = plugin->fileTypes();
            if (lst.isEmpty()) {
                return true;
            } else {
                return lst.contains(ext);
            }
        }
        return false;
    }

    bool GlobalImageLoader::canLoad(const QString &filePath) const {
        return canLoad(QFileInfo(filePath));
    }

    bool GlobalImageLoader::canLoad(const QFileInfo &file) const {
        if (!_pluginList.isEmpty()) {
            QListIterator<ImageLoaderPlugin *> i(_pluginList);
            while (i.hasNext()) {
                if (i.next()->canLoad(file)) {
                    return true;
                }
            }
        }
        return _defaultPlugin->canLoad(file);
    }

    bool GlobalImageLoader::canLoadNative(const QString &filePath) const {
        return canLoadNative(QFileInfo(filePath));
    }

    bool GlobalImageLoader::canLoadNative(const QFileInfo &file) const {
        return _defaultPlugin->canLoad(file);
    }

    QImage GlobalImageLoader::loadImage(const QString &filePath, LoadStrategy s) const {
        return loadImage(QFileInfo(filePath), s);
    }

    QImage GlobalImageLoader::loadImage(const QFileInfo &file, LoadStrategy s) const {
        switch (s) {
            case LoadDefault: {
                    if (_defaultPlugin != NULL && _defaultPlugin->canLoad(file)) {
                        return _defaultPlugin->loadImage(file);
                    }
                    break;
                }
            case LoadLinearRev: {
                    if (_defaultPlugin != NULL && _defaultPlugin->canLoad(file)) {
                        return _defaultPlugin->loadImage(file);
                    }
                    if (!_pluginList.isEmpty()) {
                        QListIterator<ImageLoaderPlugin *> i(_pluginList);
                        i.toBack();
                        while (i.hasPrevious()) {
                            ImageLoaderPlugin *p = i.previous();
                            if (fileTypeLoadable(file.suffix(), p) && p->canLoad(file)) {
                                return p->loadImage(file);
                            }
                        }
                    }
                    break;
                }
            default: {
                    if (!_pluginList.isEmpty()) {
                        QListIterator<ImageLoaderPlugin *> i(_pluginList);
                        while (i.hasNext()) {
                            ImageLoaderPlugin *p = i.next();
                            if (fileTypeLoadable(file.suffix(), p) && p->canLoad(file)) {
                                return p->loadImage(file);
                            }
                        }
                    }
                    if (_defaultPlugin != NULL && _defaultPlugin->canLoad(file)) {
                        return _defaultPlugin->loadImage(file);
                    }
                }
        }

        return QImage();
    }

    QImage GlobalImageLoader::loadImage(const QString &filePath, int plugin) const {
        return loadImage(QFileInfo(filePath), plugin);
    }

    QImage GlobalImageLoader::loadImage(const QFileInfo &file, int plugin) const {
        if (plugin >= 0 && plugin < _pluginList.size()) {
            if (fileTypeLoadable(file.suffix(), _pluginList[plugin])
                    && _pluginList[plugin]->canLoad(file)) {
                return _pluginList[plugin]->loadImage(file);
            }
        }
        return QImage();
    }

    void GlobalImageLoader::addPlugin(ImageLoaderPlugin *plugin) {
        if (plugin != NULL) {
            _pluginList.append(plugin);
        }
    }

    void GlobalImageLoader::removePlugin(int plugin) {
        if (plugin >= 0 && plugin < _pluginList.size()) {
            _pluginList.removeAt(plugin);
        }
    }

    ImageLoaderPlugin *GlobalImageLoader::getPlugin(int plugin) {
        if (plugin >= 0 && plugin < _pluginList.size()) {
            return _pluginList[plugin];
        } else if (plugin == _pluginList.size()) {
            return _defaultPlugin;
        }
        return NULL;
    }

    int GlobalImageLoader::pluginCount() {
        return _pluginList.size() + 1;
    }

    QList< QPair<QString, int> > GlobalImageLoader::mergedImageTypes() const {
        QList< QPair<QString, int> > lst;
        if (!_pluginList.isEmpty()) {
            QListIterator<ImageLoaderPlugin *> pi(_pluginList);
            int index = 0;
            while (pi.hasNext()) {
                ImageLoaderPlugin *cur = pi.next();
                QList<QString> plLst = cur->imageTypes();
                QListIterator<QString> ti(plLst);
                while (ti.hasNext()) {
                    lst.append(QPair<QString, int>(ti.next(), index));
                }
                ++index;
            }
        }

        QList<QString> defLst = _defaultPlugin->imageTypes();
        QListIterator<QString> di(defLst);
        int idx = _pluginList.size();
        while (di.hasNext()) {
            lst.append(QPair<QString, int>(di.next(), idx));
        }

        return lst;
    }

    QList< QPair<QString, int> > GlobalImageLoader::mergedFileTypes() const {
        QList< QPair<QString, int> > lst;
        if (!_pluginList.isEmpty()) {
            QListIterator<ImageLoaderPlugin *> pi(_pluginList);
            int index = 0;
            while (pi.hasNext()) {
                ImageLoaderPlugin *cur = pi.next();
                QList<QString> plLst = cur->fileTypes();
                QListIterator<QString> ti(plLst);
                while (ti.hasNext()) {
                    lst.append(QPair<QString, int>(ti.next(), index));
                }
                ++index;
            }
        }

        QList<QString> defLst = _defaultPlugin->fileTypes();
        QListIterator<QString> di(defLst);
        int idx = _pluginList.size();
        while (di.hasNext()) {
            lst.append(QPair<QString, int>(di.next(), idx));
        }

        return lst;
    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
