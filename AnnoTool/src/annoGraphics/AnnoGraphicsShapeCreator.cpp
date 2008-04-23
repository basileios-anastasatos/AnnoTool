#include "include/AnnoGraphicsShapeCreator.h"
#include "include/AnnoGraphicsRect.h"
#include "AnnoRectangle.h"

namespace anno {

    namespace graphics {

        AnnoGraphicsShapeCreator::AnnoGraphicsShapeCreator() {
        }

        AnnoGraphicsShapeCreator::~AnnoGraphicsShapeCreator() {
        }

        AnnoGraphicsShape *AnnoGraphicsShapeCreator::toGraphicsShape(
            dt::Annotation *anno) {
            AnnoGraphicsShape *shape = NULL;
            if (anno != NULL && anno->shape() != NULL) {
                switch(anno->shape()->shapeType()) {
                    case dt::ASTypeRectangle: {
                            dt::AnnoRectangle *ar = reinterpret_cast<dt::AnnoRectangle *>(anno->shape());
                            QRectF *rect = static_cast<QRectF *>(ar);
                            shape = new AnnoGraphicsRect(anno, *rect);
                            break;
                        }
                    default:
                        break;
                }
            }
            return shape;
        }

    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
