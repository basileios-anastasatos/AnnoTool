#ifndef GLOBALCONFIG_H_
#define GLOBALCONFIG_H_

#include "AllAnnoExceptions.h"

//namespace AnnoTool
namespace anno {
    //namespace Helper
    namespace helper {
        using namespace ::anno::exc;
        class GlobalConfig {
            private:
                static GlobalConfig *_me;

            private:
                GlobalConfig();
                static void setupConfig();

            public:
                virtual ~GlobalConfig();

            public:
                static const GlobalConfig *instance();
        };

    } //end namespace helper
} //end namespace anno

#endif /*GLOBALCONFIG_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
