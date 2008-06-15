#include "include/AnnoGraphicsShapeCreator.h"
#include "include/AnnoGraphicsRect.h"
#include "include/AnnoGraphicsSinglePoint.h"
#include "include/AnnoGraphicsEllipse.h"
#include "AnnoRectangle.h"
#include "AnnoSinglePoint.h"

namespace anno {

    namespace graphics {

        AnnoGraphicsShape *AnnoGraphicsShapeCreator::toGraphicsShape(dt::Annotation *anno) {
            AnnoGraphicsShape *shape = NULL;
            if (anno != NULL && anno->shape() != NULL) {
                switch (anno->shape()->shapeType()) {
                    case dt::ASTypeRectangle: {
                            dt::AnnoRectangle *ar = reinterpret_cast<dt::AnnoRectangle *>(anno->shape());
                            QRectF *rect = static_cast<QRectF *>(ar);
                            QRectF tmpRect = *rect;
                            //						tmpRect.moveTo(0.0, 0.0);
                            AnnoGraphicsRect *tmp = new AnnoGraphicsRect(anno, tmpRect);
                            //						tmp->setPos(rect->topLeft());
                            shape = tmp;
                            break;
                        }
                    case dt::ASTypeSinglePoint: {
                            shape = new AnnoGraphicsSinglePoint(anno);
                            break;
                        }
                    case dt::ASTypeEllipse: {
                            shape = new AnnoGraphicsEllipse(anno);
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
