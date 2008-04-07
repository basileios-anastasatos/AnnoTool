#include "include/GlobalConfig.h"

//namespace AnnoTool
namespace anno {
    //namespace Helper
    namespace helper {
        GlobalConfig *GlobalConfig::_me = NULL;

        GlobalConfig::GlobalConfig() {
        }

        GlobalConfig::~GlobalConfig() {
        }

        void GlobalConfig::setupConfig() {
            _me = new GlobalConfig();
        }

        const GlobalConfig *GlobalConfig::instance() {
            if(_me == NULL) {
                setupConfig();
            }
            return _me;
        }

    } //end namespace helper
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
