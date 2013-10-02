#include "include/AnnoGraphicsSegmentation.h"
#include <QMap>
#include <QRectF>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include "importGlobals.h"
#include <QVariant>

#include "AnnoAvClassList.h"
#include "Segmentation.h"

#include <iostream>

namespace anno {
    namespace graphics {

        AnnoGraphicsSegmentation::AnnoGraphicsSegmentation(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsRectItem(parent), AnnoGraphicsShape(anno) {
            setupAppearance();
            setRect(*annoSegmentation());
            initControlPoints();
            _bShowMask = false;
            _bMove = false;
        }

        AnnoGraphicsSegmentation::~AnnoGraphicsSegmentation() {
        }

        QRectF AnnoGraphicsSegmentation::mapRectToParent(const QRectF &r) const {
            QPointF p0 = mapToParent(r.topLeft());
            QSizeF s = r.size();

            return QRectF(p0, s);
        }

        void AnnoGraphicsSegmentation::initControlPoints() {
            insertControlPoint(0, new AnnoGraphicsControlPoint(this, 0));
            insertControlPoint(1, new AnnoGraphicsControlPoint(this, 1));
            insertControlPoint(2, new AnnoGraphicsControlPoint(this, 2));
            insertControlPoint(3, new AnnoGraphicsControlPoint(this, 3));
            setControlPointsVisible(false);
            validateCpPos();
        }

        void AnnoGraphicsSegmentation::validateCpPos() {
            QRectF rect = *annoSegmentation();
            QPointF p = rect.topLeft();
            moveControlPointTo(0, p.x(), p.y());
            p = rect.bottomLeft();
            moveControlPointTo(1, p.x(), p.y());
            p = rect.bottomRight();
            moveControlPointTo(2, p.x(), p.y());
            p = rect.topRight();
            moveControlPointTo(3, p.x(), p.y());
        }

        QGraphicsItem *AnnoGraphicsSegmentation::graphicsItem() {
            return this;
        }

        void AnnoGraphicsSegmentation::shapeMoveBy(qreal deltaX, qreal deltaY) {
            _bMove = true;
            dt::AnnoSegmentation *bBox = annoSegmentation();
            QImage *qImg = bBox->getImage();
            if(NULL != qImg) {
                _bShowMask = false;
            }

            QPointF delta(deltaX, deltaY);
            QRectF tmpRect = mapRectToParent(*annoSegmentation());
            QRectF parRect = parentItem()->boundingRect();
            tmpRect.moveTo(tmpRect.topLeft() + delta);

            if (parRect.contains(tmpRect)) {
                prepareGeometryChange();
                *annoSegmentation() = tmpRect;
                setRect(tmpRect);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsSegmentation::shapeSizeBy(qreal facX, qreal facY) {
            //TODO must be implemented!
        }

        dt::AnnoShapeType AnnoGraphicsSegmentation::shapeType() const {
            return dt::ASTypeSegmentation;
        }

        void AnnoGraphicsSegmentation::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent) {
            dt::Annotation *anno = relatedAnno();
            if(GlobalProjectManager::instance()->isAnnoSelected(anno)) {
                GlobalToolManager::instance()->triggerShapeContextMenu(relatedAnno());
            }
        }

        void AnnoGraphicsSegmentation::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: mousePressEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsSegmentation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: mouseReleaseEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
            dt::AnnoSegmentation *bBox = annoSegmentation();
            QImage *qImg = bBox->getImage();
            if(NULL != qImg && _bMove) {
                changeBoundingBox();
                _bShowMask = true;
                _bMove = false;
            }
        }

        void AnnoGraphicsSegmentation::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: mouseDoubleClickEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseDoubleClickEvent(this, event);
            }
        }

        void AnnoGraphicsSegmentation::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsSegmentation::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: hoverEnterEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverEnterEvent(this, event);
            }
        }

        void AnnoGraphicsSegmentation::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: hoverLeaveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverLeaveEvent(this, event);
            }
        }

        void AnnoGraphicsSegmentation::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: hoverMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverMoveEvent(this, event);
            }
        }

        void AnnoGraphicsSegmentation::keyPressEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: keyPressEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyPressEvent(this, event);
            }
        }

        void AnnoGraphicsSegmentation::keyReleaseEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: keyReleaseEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyReleaseEvent(this, event);
            }
        }

        QVariant AnnoGraphicsSegmentation::itemChange(GraphicsItemChange change, const QVariant &value) {
            if(change == QGraphicsItem::ItemSelectedChange) {
                if(value.toBool()) {
                    // buildSegmentationImage is done only once when the segmentation is selected for the first time
                    dt::AnnoSegmentation *bBox = annoSegmentation();
                    QImage *qImg = bBox->getImage();
                    if (NULL == qImg) {
                        dt::Segmentation *segm = dynamic_cast<dt::Segmentation *>(_anno);
                        if (segm) {
                            segm->buildSegmentationImage();
                        }
                    }

                    _bShowMask = true;
                    setControlPointsVisible(true);
                } else {
                    _bShowMask = false;
                    setControlPointsVisible(false);
                }
            }
            return QGraphicsRectItem::itemChange(change, value);
        }

        void AnnoGraphicsSegmentation::paint(QPainter *painter,
                                             const QStyleOptionGraphicsItem *option, QWidget *widget) {
            dt::AnnoSegmentation *bBox = annoSegmentation();
            if (NULL == bBox) {
                return;
            }
            QImage *qImg = bBox->getImage();
            QRectF imgRect = bBox->boundingRect();
            if (NULL != qImg && _bShowMask) {
                painter->drawImage(imgRect, *qImg);
            }

            _shapeConfig.applyShapeConfig(painter, isSelected());
            painter->drawRect(rect());
        }

        void AnnoGraphicsSegmentation::setupAppearance() {
            setFlag(QGraphicsItem::ItemIsSelectable);
            setFlag(QGraphicsItem::ItemIsFocusable);
            setAcceptHoverEvents(true);
            setVisible(true);
            setToolTip(_anno->annoInfo());
            _bShowMask = false;
        }

        dt::AnnoSegmentation *AnnoGraphicsSegmentation::annoSegmentation() {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeSegmentation) {
                return dynamic_cast<dt::AnnoSegmentation *>(_anno->shape());
            }
            return NULL;
        }

        const dt::AnnoSegmentation *AnnoGraphicsSegmentation::annoSegmentation() const {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeSegmentation) {
                return dynamic_cast<dt::AnnoSegmentation *>(_anno->shape());
            }
            return NULL;
        }

        void AnnoGraphicsSegmentation::cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_SEGMENTATION: cpMousePressEvent on CP %1").arg(index));
            dt::AnnoSegmentation *bBox = annoSegmentation();
            QImage *qImg = bBox->getImage();
            if(NULL != qImg) {
                _bShowMask = false;
            }
        }

        void AnnoGraphicsSegmentation::cpMouseReleaseEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_SEGMENTATION: cpMouseReleaseEvent on CP %1").arg(index));
            prepareGeometryChange();
            *annoSegmentation() = annoSegmentation()->normalized();
            setRect(*annoSegmentation());
            validateCpPos();
            _anno->setModified(true);
            setToolTip(_anno->annoInfo());
            dt::AnnoSegmentation *bBox = annoSegmentation();
            QImage *qImg = bBox->getImage();
            if(NULL != qImg) {
                changeBoundingBox();
                _bShowMask = true;
            }
        }

        void AnnoGraphicsSegmentation::cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_SEGMENTATION: cpMouseMoveEvent on CP %1").arg(index));
            qreal deltaX = event->scenePos().x() - event->lastScenePos().x();
            qreal deltaY = event->scenePos().y() - event->lastScenePos().y();
            QPointF delta(deltaX, deltaY);

            QRectF rect = *annoSegmentation();
            QPointF nPoint;
            switch (index) {
                case 0:
                    nPoint = rect.topLeft();
                    nPoint += delta;
                    rect.setTopLeft(nPoint);
                    break;
                case 1:
                    nPoint = rect.bottomLeft();
                    nPoint += delta;
                    rect.setBottomLeft(nPoint);
                    break;
                case 2:
                    nPoint = rect.bottomRight();
                    nPoint += delta;
                    rect.setBottomRight(nPoint);
                    break;
                case 3:
                    nPoint = rect.topRight();
                    nPoint += delta;
                    rect.setTopRight(nPoint);
                    break;
            }
            QRectF tmpRect = mapRectToParent(rect);
            QRectF parRect = parentItem()->boundingRect();
            if (parRect.contains(tmpRect)) {
                prepareGeometryChange();
                *annoSegmentation() = tmpRect;
                setRect(tmpRect);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsSegmentation::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: exMouseMoveEvent.");
            mouseMoveEvent(event);
        }

        void AnnoGraphicsSegmentation::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: exMousePressEvent.");
            mousePressEvent(event);
        }

        void AnnoGraphicsSegmentation::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_SEGMENTATION: exMouseReleaseEvent.");
            mouseReleaseEvent(event);
        }

        void AnnoGraphicsSegmentation::changeBoundingBox() {
            dt::AnnoSegmentation *bBox = annoSegmentation();
            QImage *qImg = bBox->getImage();
            if(NULL != qImg) {
                QRectF imgRect = bBox->boundingRect();
                //tell to Segmentation that BBox has changed
                dt::Segmentation *segm = dynamic_cast<dt::Segmentation *>(_anno);
                if (!segm) {
                    return;
                }
                segm->recalculateSegmentation(imgRect);
            }
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
