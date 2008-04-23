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

} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
