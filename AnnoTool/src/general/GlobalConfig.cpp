#include "include/GlobalConfig.h"

//namespace AnnoTool
namespace anno {
    GlobalConfig *GlobalConfig::_me = NULL;
    const FileExtensions GlobalConfig::fileExt("atp", "atc", "ata");
    const ShapeColors GlobalConfig::shapeColors;

    GlobalConfig::GlobalConfig() {
    }

    GlobalConfig::~GlobalConfig() {
    }

    void GlobalConfig::setupConfig() {
        _me = new GlobalConfig();
    }

    const GlobalConfig *GlobalConfig::instance() {
        if (_me == NULL) {
            setupConfig();
        }
        return _me;
    }

    int GlobalConfig::getSettingInt(const QString &s) const throw(NoSuchElementException *) {
        if(s == "logging.loglevel") {
            return 1;
        }

        if (_settingsInt.contains(s)) {
            return _settingsInt[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    double GlobalConfig::getSettingDouble(const QString &s) const
    throw(NoSuchElementException *) {
        if (_settingsDouble.contains(s)) {
            return _settingsDouble[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    QString GlobalConfig::getSettingString(const QString &s) const
    throw(NoSuchElementException *) {
        if (_settingsString.contains(s)) {
            return _settingsString[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

    QFileInfo GlobalConfig::getSettingFile(const QString &s) const
    throw(NoSuchElementException *) {
        if (_settingsFile.contains(s)) {
            return _settingsFile[s];
        } else {
            throw new NoSuchElementException(__FILE__, __LINE__, s);
        }
    }

} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
