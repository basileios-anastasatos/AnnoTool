#include "include/AnnoGraphicsShapeCreator.h"
#include "include/AnnoGraphicsRect.h"
#include "include/AnnoGraphicsSinglePoint.h"
#include "include/AnnoGraphicsEllipse.h"
#include "include/AnnoGraphicsPolygon.h"
#include "include/AnnoGraphicsBoundingBox.h"
#include "include/AnnoGraphicsBrush.h"
#include "importGlobals.h"

namespace anno {
    namespace graphics {

        AnnoGraphicsShape *AnnoGraphicsShapeCreator::toGraphicsShape(dt::Annotation *anno) {
            AnnoGraphicsShape *shape = NULL;
            if (anno != NULL && anno->shape() != NULL) {
                switch (anno->shape()->shapeType()) {
                    case dt::ASTypeRectangle: {
                            shape = new AnnoGraphicsRect(anno);
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
                    case dt::ASTypePolygon: {
                            shape = new AnnoGraphicsPolygon(anno);
                            break;
                        }
                    case dt::ASTypeBoundingBox: {
                            shape = new AnnoGraphicsBoundingBox(anno);
                            break;
                        }
                    case dt::ASTypeBrush: {
                            shape = new AnnoGraphicsBrush(anno);
                            break;
                        }
                    default:
                        GlobalLogger::instance()->logError("Cannot create graphics representation of annotaion! Unknown annotation type!");
                        break;
                }
            } else {
                GlobalLogger::instance()->logError("Cannot create graphics representation of annotaion!");
            }
            return shape;
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
