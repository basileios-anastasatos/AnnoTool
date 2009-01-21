#include "include/AnnoFilterRule.h"

namespace anno {
    namespace filter {

        bool AnnoFilterRule::eval(const dt::Annotation *anno) const throw(exc::IllegalStateException *) {
            if(_useConstValue) {
                return _constValue;
            }
            return evalInternal(anno);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
