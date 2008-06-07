#include "include/ToolSinglePoint.h"
#include "importGlobals.h"

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoSinglePoint.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsShapeCreator.h"

namespace anno {
    namespace graphics {

        ToolSinglePoint::ToolSinglePoint(AnnoGraphicsScene *scene) :
            GraphicsTool(scene) {
        }

        ToolSinglePoint::~ToolSinglePoint() {
        }

        bool ToolSinglePoint::handlesCp() const {
            return false;
        }

        bool ToolSinglePoint::handlesShape() const {
            return false;
        }

        bool ToolSinglePoint::handlesImage() const {
            return true;
        }

        void ToolSinglePoint::mousePressEvent(AnnoGraphicsShape *shape,
                                              QGraphicsSceneMouseEvent *event) {
            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMousePressEvent(event);
            }
        }

        void ToolSinglePoint::mousePressEvent(AnnoGraphicsPixmap *img,
                                              QGraphicsSceneMouseEvent *event) {
            event->accept();
            dt::AnnoFileData *annoFile = GlobalProjectManager::instance()->selectedFile();
            if (annoFile != NULL) {
                dt::AnnoSinglePoint *asp = new dt::AnnoSinglePoint();
                *asp = img->mapFromScene(event->scenePos());
                dt::Annotation *anno = new dt::Annotation();
                anno->setAnnoId(QUuid::createUuid());
                anno->setShape(asp);
                annoFile->addAnnotation(anno);

                AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(anno);
                if (s != NULL) {
                    _scene->addAnnoShape(s);
                    GlobalProjectManager::instance()->setSelectedAnnoRow(anno->annoId());
                }
                //AnnoToolMainWindow::updateUI();
            }
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
