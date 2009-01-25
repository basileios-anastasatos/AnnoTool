#include <iostream>

#include "include/ToolSinglePoint.h"
#include "importGlobals.h"

#include "AnnoGraphicsScene.h"
#include "AnnoFileData.h"
#include "Annotation.h"
#include "AnnoSinglePoint.h"
#include "AnnoGraphicsShape.h"
#include "AnnoGraphicsShapeCreator.h"
#include <QGraphicsView>

namespace anno {
    namespace graphics {

        ToolSinglePoint::ToolSinglePoint(QGraphicsView *view, AnnoGraphicsScene *scene) :
            GraphicsTool(view, scene) {
            _cursorNormal = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curSPoint")), 0, 0);
            _cursorActive = QCursor(QPixmap::fromImage(QImage(":/res/cursors/curSPoint_active")), 0, 0);
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

        void ToolSinglePoint::switchDefaultTool() {
            GlobalToolManager::instance()->selectToolDefault();
        }

        void ToolSinglePoint::mousePressEvent(AnnoGraphicsShape *shape,
                                              QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (shape->parentImage() != NULL) {
                shape->parentImage()->exMousePressEvent(event);
            }
        }

        void ToolSinglePoint::mousePressEvent(AnnoGraphicsPixmap *img,
                                              QGraphicsSceneMouseEvent *event) {
            if(event->button() != Qt::LeftButton) {
                return;
            }

            if (_view != NULL) {
                _prevCursors.push(_view->cursor());
                _view->setCursor(_cursorActive);
            }

            dt::AnnoFileData *annoFile = GlobalProjectManager::instance()->selectedFile();
            if (annoFile != NULL) {
                /* create annotation object */

                dt::AnnoSinglePoint *asp = new dt::AnnoSinglePoint();
                *asp = img->mapFromScene(event->scenePos());

                dt::Annotation *anno = new dt::Annotation();
                anno->setAnnoId(QUuid::createUuid());
                anno->setShape(asp);

                std::cout << "ToolSinglePoint, PoseMode: " <<
                          GlobalProjectManager::instance()->isPoseMode() << std::endl;

                /* MA: "Pose Mode" */
                QUuid parentId;
                anno::dt::Annotation *parentAnno = NULL;

                if (GlobalProjectManager::instance()->isPoseMode()) {

                    GlobalProjectManager *pm = GlobalProjectManager::instance();
                    parentId = pm->lastSelNotPointAnno();

                    if (!parentId.isNull()) {
                        parentAnno = pm->selectedFile()->getAnnotation(parentId);
                        assert(parentAnno != NULL);

                        std::cout << "Pose Mode: set parent to " <<
                                  parentId.toString().toStdString() << std::endl;

                        anno->setAnnoParent(parentId);

                        /* find max id of other children */
                        QList<QUuid> children_list = parentAnno->annoChildren();
                        QListIterator<QUuid> it(children_list);
                        int max_child_id = -1;

                        //std::cout << "parent: " <<
                        //  parentAnno->annoId().toString().toStdString() << std::endl;

                        //std::cout << "\tno children: " << children_list.isEmpty() << std::endl;

                        while(it.hasNext()) {
                            QUuid childUuid = it.next();
                            //std::cout << "\tchild: " << childUuid.toString().toStdString() << std::endl;

                            Annotation *childAnno = pm->selectedFile()->getAnnotation(childUuid);

                            QString qsId;
                            if (childAnno->getClassAttributeValue(NATIVE_CLASS_POSEPOINT,
                                                                  NATIVE_POSEPOINT_ID_ATTR,
                                                                  qsId)) {
                                bool ok = false;
                                int cur_id = qsId.toInt(&ok);
                                assert(ok);

                                if (cur_id > max_child_id) {
                                    max_child_id = cur_id;
                                }
                            }
                        }// children

                        anno->addClass(NATIVE_CLASS_POSEPOINT);
                        anno::dt::AnnoAttribute atr(anno,
                                                    NATIVE_POSEPOINT_ID_ATTR,
                                                    NATIVE_CLASS_POSEPOINT,
                                                    QString::number(max_child_id + 1));
                        anno->addAttribute(atr);

                        std::cout << "new pose point: " << max_child_id + 1 << std::endl;

                        /* update parent's children list, is this the right place to do it? */
                        parentAnno->addAnnoChild(anno->annoId());

                    } else {
                        std::cout << "Pose Mode: no selected annotation" << std::endl;
                    }

                } else {

                    /* handle "Lock Parent" mode */

                    parentId = GlobalToolManager::instance()->getLockedAnno();

                    if(!parentId.isNull()) {
                        parentAnno = GlobalProjectManager::instance()->selectedFile()->getAnnotation(parentId);
                        anno->setAnnoParent(parentId);
                    }
                }

                annoFile->addAnnotation(anno);

                /* add graphics to the scene */
                AnnoGraphicsShape *s = AnnoGraphicsShapeCreator::toGraphicsShape(anno);
                if (s != NULL) {
                    _scene->addAnnoShape(s);

                    /* MA BEGIN: make sure child items are drawn over the parents */
                    if (!parentId.isNull()) {
                        AnnoGraphicsShape *parentShape = _scene->getShapeByAnnoId(parentId);

                        assert(parentShape != 0);
                        assert(parentShape->graphicsItem() != 0);
                        assert(s->graphicsItem() != 0);

                        s->graphicsItem()->setParentItem(parentShape->graphicsItem());
                        qreal parentZ = parentShape->graphicsItem()->zValue();
                        s->graphicsItem()->setZValue(parentZ + 1);
                    }

//                                         if (GlobalProjectManager::instance()->isPoseMode())
//                                           _scene->bringToFront(anno->annoId());
                    /* MA END */

                    GlobalProjectManager::instance()->setSelectedAnnoRow(anno->annoId());
                }
                //AnnoToolMainWindow::updateUI();
            }
        }

        void ToolSinglePoint::mouseReleaseEvent(AnnoGraphicsPixmap *img,
                                                QGraphicsSceneMouseEvent *event) {
            if(event->button() == Qt::RightButton) {
                switchDefaultTool();
                return;
            } else if(event->button() != Qt::LeftButton) {
                return;
            }

            if (_view != NULL) {
                _view->setCursor(_prevCursors.pop());
            }
        }

        void ToolSinglePoint::hoverEnterEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(_cursorNormal);
            }
        }

        void ToolSinglePoint::hoverLeaveEvent(AnnoGraphicsPixmap *img, QGraphicsSceneHoverEvent *event) {
            if (_view != NULL) {
                _view->setCursor(Qt::ArrowCursor);
            }
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
