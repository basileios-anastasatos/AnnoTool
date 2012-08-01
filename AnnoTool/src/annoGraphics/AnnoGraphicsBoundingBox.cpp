#include "include/AnnoGraphicsBoundingBox.h"
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

#include "AnnoAvClassList.h"
#include "Segmentation.h"

#include <iostream>

namespace anno {
    namespace graphics {

        AnnoGraphicsBoundingBox::AnnoGraphicsBoundingBox(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsRectItem(parent), AnnoGraphicsShape(anno) {
            setupAppearance();
            setRect(*annoBoundingBox());
            initControlPoints();
            _bShowMask = false;
            _bMove = false;
        }

        AnnoGraphicsBoundingBox::~AnnoGraphicsBoundingBox() {
        }

        QRectF AnnoGraphicsBoundingBox::mapRectToParent(const QRectF &r) const {
            QPointF p0 = mapToParent(r.topLeft());
            QSizeF s = r.size();

            return QRectF(p0, s);
        }

        void AnnoGraphicsBoundingBox::initControlPoints() {
            insertControlPoint(0, new AnnoGraphicsControlPoint(this, 0));
            insertControlPoint(1, new AnnoGraphicsControlPoint(this, 1));
            insertControlPoint(2, new AnnoGraphicsControlPoint(this, 2));
            insertControlPoint(3, new AnnoGraphicsControlPoint(this, 3));
            setControlPointsVisible(false);
            validateCpPos();
        }

        void AnnoGraphicsBoundingBox::validateCpPos() {
            QRectF rect = *annoBoundingBox();
            QPointF p = rect.topLeft();
            moveControlPointTo(0, p.x(), p.y());
            p = rect.bottomLeft();
            moveControlPointTo(1, p.x(), p.y());
            p = rect.bottomRight();
            moveControlPointTo(2, p.x(), p.y());
            p = rect.topRight();
            moveControlPointTo(3, p.x(), p.y());
        }

        QGraphicsItem *AnnoGraphicsBoundingBox::graphicsItem() {
            return this;
        }

        void AnnoGraphicsBoundingBox::shapeMoveBy(qreal deltaX, qreal deltaY) {
            _bMove = true;
            dt::AnnoBoundingBox *bBox = annoBoundingBox();
            QImage *qImg = bBox->getImage();
            if(NULL != qImg) {
                _bShowMask = false;
            }

            QPointF delta(deltaX, deltaY);
            QRectF tmpRect = mapRectToParent(*annoBoundingBox());
            QRectF parRect = parentItem()->boundingRect();
            tmpRect.moveTo(tmpRect.topLeft() + delta);

            if (parRect.contains(tmpRect)) {
                prepareGeometryChange();
                *annoBoundingBox() = tmpRect;
                setRect(tmpRect);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsBoundingBox::shapeSizeBy(qreal facX, qreal facY) {
            //TODO must be implemented!
        }

        dt::AnnoShapeType AnnoGraphicsBoundingBox::shapeType() const {
            return dt::ASTypeBoundingBox;
        }

        void AnnoGraphicsBoundingBox::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent) {
            dt::Annotation *anno = relatedAnno();
            if(GlobalProjectManager::instance()->isAnnoSelected(anno)) {
                GlobalToolManager::instance()->triggerShapeContextMenu(relatedAnno());
            }
        }

        void AnnoGraphicsBoundingBox::mousePressEvent(QGraphicsSceneMouseEvent *event) {
//			dt::AnnoBoundingBox* bBox = annoBoundingBox();
//			QImage* qImg = bBox->getImage();
//			if(NULL != qImg)
//				_bShowMask = false;
            GlobalLogger::instance()->logDebug("AG_RECT: mousePressEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mouseReleaseEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
            dt::AnnoBoundingBox *bBox = annoBoundingBox();
            QImage *qImg = bBox->getImage();
            if(NULL != qImg && _bMove) {
                changeBoundingBox();
                _bShowMask = true;
                _bMove = false;
            }
        }

        void AnnoGraphicsBoundingBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mouseDoubleClickEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseDoubleClickEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: hoverEnterEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverEnterEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: hoverLeaveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverLeaveEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: hoverMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverMoveEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::keyPressEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: keyPressEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyPressEvent(this, event);
            }
        }

        void AnnoGraphicsBoundingBox::keyReleaseEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_RECT: keyReleaseEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyReleaseEvent(this, event);
            }
        }

        QVariant AnnoGraphicsBoundingBox::itemChange(GraphicsItemChange change, const QVariant &value) {
            if(change == QGraphicsItem::ItemSelectedChange) {
                if(value.toBool()) {
                    // buildSegmentationImage is done only once when the segmentation is selected for the first time
                    dt::AnnoBoundingBox *bBox = annoBoundingBox();
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

        void AnnoGraphicsBoundingBox::paint(QPainter *painter,
                                            const QStyleOptionGraphicsItem *option, QWidget *widget) {
            dt::AnnoBoundingBox *bBox = annoBoundingBox();
            QImage *qImg = bBox->getImage();
            QRectF imgRect = bBox->boundingRect();
            if (NULL != qImg && _bShowMask) {
                painter->drawImage(imgRect, *qImg);
            }

            _shapeConfig.applyShapeConfig(painter, isSelected());
            painter->drawRect(rect());
        }

        void AnnoGraphicsBoundingBox::setupAppearance() {
            setFlag(QGraphicsItem::ItemIsSelectable);
            setFlag(QGraphicsItem::ItemIsFocusable);
            setAcceptsHoverEvents(true);
            setVisible(true);
            setToolTip(_anno->annoInfo());
            _bShowMask = false;
        }

        dt::AnnoBoundingBox *AnnoGraphicsBoundingBox::annoBoundingBox() {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeBoundingBox) {
                return dynamic_cast<dt::AnnoBoundingBox *>(_anno->shape());
            }
            return NULL;
        }

        const dt::AnnoBoundingBox *AnnoGraphicsBoundingBox::annoBoundingBox() const {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypeBoundingBox) {
                return dynamic_cast<dt::AnnoBoundingBox *>(_anno->shape());
            }
            return NULL;
        }

        void AnnoGraphicsBoundingBox::cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_BOUNDING_BOX: cpMousePressEvent on CP %1").arg(index));
            dt::AnnoBoundingBox *bBox = annoBoundingBox();
            QImage *qImg = bBox->getImage();
            if(NULL != qImg) {
                _bShowMask = false;
            }
        }

        void AnnoGraphicsBoundingBox::cpMouseReleaseEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_BOUNDING_BOX: cpMouseReleaseEvent on CP %1").arg(index));
            prepareGeometryChange();
            *annoBoundingBox() = annoBoundingBox()->normalized();
            setRect(*annoBoundingBox());
            validateCpPos();
            _anno->setModified(true);
            setToolTip(_anno->annoInfo());
            dt::AnnoBoundingBox *bBox = annoBoundingBox();
            QImage *qImg = bBox->getImage();
            if(NULL != qImg) {
                changeBoundingBox();
                _bShowMask = true;
            }
        }

        void AnnoGraphicsBoundingBox::cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_BOUNDING_BOX: cpMouseMoveEvent on CP %1").arg(index));
            qreal deltaX = event->scenePos().x() - event->lastScenePos().x();
            qreal deltaY = event->scenePos().y() - event->lastScenePos().y();
            QPointF delta(deltaX, deltaY);

            QRectF rect = *annoBoundingBox();
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
                *annoBoundingBox() = tmpRect;
                setRect(tmpRect);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsBoundingBox::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_BOUNDING_BOX: exMouseMoveEvent.");
            mouseMoveEvent(event);
        }

        void AnnoGraphicsBoundingBox::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_BOUNDING_BOX: exMousePressEvent.");
            mousePressEvent(event);
        }

        void AnnoGraphicsBoundingBox::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_BOUNDING_BOX: exMouseReleaseEvent.");
            mouseReleaseEvent(event);
        }

        void AnnoGraphicsBoundingBox::changeBoundingBox() {
            dt::AnnoBoundingBox *bBox = annoBoundingBox();
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
