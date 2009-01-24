#include "include/AnnoGraphicsSinglePoint.h"
#include "importGlobals.h"

#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>

namespace anno {
    namespace graphics {

        AnnoGraphicsSinglePoint::AnnoGraphicsSinglePoint(dt::Annotation *anno,
                QGraphicsItem *parent) :
            QGraphicsItem(parent), AnnoGraphicsShape(anno) {
            setupAppearance();
        }

        AnnoGraphicsSinglePoint::AnnoGraphicsSinglePoint(dt::Annotation *anno,
                const QPointF &point, QGraphicsItem *parent) :
            QGraphicsItem(parent), AnnoGraphicsShape(anno) {
            *annoSinglePoint() = point;
            setupAppearance();
        }

        AnnoGraphicsSinglePoint::AnnoGraphicsSinglePoint(dt::Annotation *anno, qreal x,
                qreal y, QGraphicsItem *parent) :
            QGraphicsItem(parent), AnnoGraphicsShape(anno) {
            dt::AnnoSinglePoint *asp = annoSinglePoint();
            asp->setX(x);
            asp->setY(y);
            setupAppearance();
        }

        AnnoGraphicsSinglePoint::~AnnoGraphicsSinglePoint() {
        }

        void AnnoGraphicsSinglePoint::setupAppearance() {
            setFlag(QGraphicsItem::ItemIsSelectable);
            setAcceptsHoverEvents(true);
            setVisible(true);
            setToolTip(_anno->annoInfo());
        }

        dt::AnnoSinglePoint *AnnoGraphicsSinglePoint::annoSinglePoint() {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeSinglePoint) {
                return reinterpret_cast<dt::AnnoSinglePoint *>(_anno->shape());
            }
            return NULL;
        }

        const dt::AnnoSinglePoint *AnnoGraphicsSinglePoint::annoSinglePoint() const {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeSinglePoint) {
                return reinterpret_cast<dt::AnnoSinglePoint *>(_anno->shape());
            }
            return NULL;
        }

        void AnnoGraphicsSinglePoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent) {
            dt::Annotation *anno = relatedAnno();
            if(GlobalProjectManager::instance()->isAnnoSelected(anno)) {
                GlobalToolManager::instance()->triggerShapeContextMenu(relatedAnno());
            }
        }

        void AnnoGraphicsSinglePoint::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: mousePressEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsSinglePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: mouseReleaseEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsSinglePoint::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: mouseDoubleClickEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseDoubleClickEvent(this, event);
            }
        }

        void AnnoGraphicsSinglePoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsSinglePoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: hoverEnterEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverEnterEvent(this, event);
            }
        }

        void AnnoGraphicsSinglePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: hoverLeaveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverLeaveEvent(this, event);
            }
        }

        void AnnoGraphicsSinglePoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: hoverMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverMoveEvent(this, event);
            }
        }

        void AnnoGraphicsSinglePoint::keyPressEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: keyPressEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyPressEvent(this, event);
            }
        }

        void AnnoGraphicsSinglePoint::keyReleaseEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: keyReleaseEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyReleaseEvent(this, event);
            }
        }

        QVariant AnnoGraphicsSinglePoint::itemChange(GraphicsItemChange change, const QVariant &value) {
            if (change == QGraphicsItem::ItemSelectedChange) {
            }
            return QGraphicsItem::itemChange(change, value);
        }

        QGraphicsItem *AnnoGraphicsSinglePoint::graphicsItem() {
            return this;
        }

        void AnnoGraphicsSinglePoint::shapeMoveBy(qreal deltaX, qreal deltaY) {
            dt::AnnoSinglePoint *asp = annoSinglePoint();
            if (asp != NULL) {
                QPointF delta(deltaX, deltaY);
                QPointF tmpPoint = mapToParent(*asp);
                tmpPoint += delta;

                QRectF parRect = parentItem()->boundingRect();
                if (parRect.contains(tmpPoint)) {
                    prepareGeometryChange();
                    *asp = tmpPoint;
                    _anno->setModified(true);
                    setToolTip(_anno->annoInfo());
                }
            }
        }

        void AnnoGraphicsSinglePoint::shapeSizeBy(qreal facX, qreal facY) {
            // Nothing to do here!
        }

        dt::AnnoShapeType AnnoGraphicsSinglePoint::shapeType() const {
            return dt::ASTypeSinglePoint;
        }

        QRectF AnnoGraphicsSinglePoint::boundingRect() const {
            const dt::AnnoSinglePoint *asp = annoSinglePoint();

            /* MA: make markers smaller  */
            //return QRectF(asp->x() - 25, asp->y() - 25, 50, 50);
            int marker_width = 10;
            int marker_height = 10;

            return QRectF(asp->x() - marker_width / 2, asp->y() - marker_height / 2,
                          marker_width, marker_height);


        }

        void AnnoGraphicsSinglePoint::paint(QPainter *painter,
                                            const QStyleOptionGraphicsItem *option, QWidget *widget) {
            //TODO nice painting!!
            QRectF brect = boundingRect();
            QRectF
            ibrect(brect.x() + 5.5, brect.y() + 5.5, brect.width() - 11.0, brect.height() - 11.0);
            QPointF np = *annoSinglePoint();
            GlobalLogger::instance()->logDebug(QString("AG_SPOINT: paint (%1,%2, %3,%4)").arg(brect.x()).arg(brect.y()).arg(brect.width()).arg(brect.height()));
            if (isSelected()) {
                QPen penNone(QColor(0, 0, 0, 0));
                penNone.setWidth(0);

                QPen penInner(QColor(0, 0, 0, 255));
                penInner.setWidth(1);

                QPen penOuter(QColor(255, 255, 255, 255));
                penOuter.setWidth(1);

                QPen penNormal(QColor(30, 30, 255, 255));
                penNormal.setWidth(2);

                QPen penFat(QColor(255, 255, 0, 128));
                penFat.setWidth(10);

                QBrush brushNone(QColor(0, 0, 0, 255));
                QBrush brushColor(QColor(255, 255, 0, 200));

                painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
                painter->setBrush(brushColor);
                painter->setPen(penNormal);
                painter->drawEllipse(brect);
                painter->setPen(penInner);
                painter->drawEllipse((int)np.x() - 1, (int)np.y() - 1, 2, 2);
                painter->setPen(penOuter);
                painter->drawEllipse((int)np.x() - 2, (int)np.y() - 2, 4, 4);
            } else {
                QPen penNone(QColor(0, 0, 0, 0));
                penNone.setWidth(0);

                QPen penInner(QColor(0, 0, 0, 255));
                penInner.setWidth(1);

                QPen penOuter(QColor(255, 255, 255, 255));
                penOuter.setWidth(1);

                QPen penNormal(QColor(30, 30, 255, 255));
                penNormal.setWidth(1);

                QPen penFat(QColor(255, 255, 0, 128));
                penFat.setWidth(10);

                QBrush brushNone(QColor(0, 0, 0, 255));
                QBrush brushColor(QColor(255, 255, 0, 128));

                painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
                painter->setBrush(brushColor);
                painter->setPen(penNormal);
                painter->drawEllipse(brect);

                //				painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
                //				painter->setBrush(brushNone);
                //				painter->setPen(penNone);
                //				painter->drawEllipse(ibrect);
                //				painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
                //


                painter->setPen(penInner);
                painter->drawEllipse((int)np.x() - 1, (int)np.y() - 1, 2, 2);
                painter->setPen(penOuter);
                painter->drawEllipse((int)np.x() - 2, (int)np.y() - 2, 4, 4);

            }
        }

        void AnnoGraphicsSinglePoint::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: exMouseMoveEvent.");
            mouseMoveEvent(event);
        }

        void AnnoGraphicsSinglePoint::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: exMousePressEvent.");
            mousePressEvent(event);
        }

        void AnnoGraphicsSinglePoint::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SPOINT: exMouseReleaseEvent.");
            mouseReleaseEvent(event);
        }

    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
