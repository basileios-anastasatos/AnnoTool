#ifndef ANNOGRAPHICSSHAPECREATOR_H_
#define ANNOGRAPHICSSHAPECREATOR_H_

#include "Annotation.h"
#include "AnnoGraphicsShape.h"

namespace anno {
    namespace graphics {

        class AnnoGraphicsShapeCreator {
            private:
                AnnoGraphicsShapeCreator() {
                }

                ~AnnoGraphicsShapeCreator() {
                }

            public:
                static AnnoGraphicsShape *toGraphicsShape(dt::Annotation *anno);
        };

    }
}

#endif /*ANNOGRAPHICSSHAPECREATOR_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
