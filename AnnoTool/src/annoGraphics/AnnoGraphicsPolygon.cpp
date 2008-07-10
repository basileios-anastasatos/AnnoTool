#include "AnnoGraphicsPolygon.h"
#include <QRectF>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include "importGlobals.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>

namespace anno {
    namespace graphics {

        AnnoGraphicsPolygon::AnnoGraphicsPolygon(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsPolygonItem(parent), AnnoGraphicsShape(anno), _drawClosed(true) {
            setupAppearance();
            setPolygon(*annoPolygon());
            initControlPoints();
        }

        AnnoGraphicsPolygon::~AnnoGraphicsPolygon() {
        }

        void AnnoGraphicsPolygon::setupAppearance() {
            ShapeConfig sc = GlobalConfig::instance()->getShapeConfig("polygon");
            setPen(sc.penNormal);
            setBrush(sc.brushNormal);
            setFlag(QGraphicsItem::ItemIsSelectable);
            setVisible(true);
            setAcceptsHoverEvents(true);
            setToolTip(QString("%1\n%2").arg(_anno->annoIdAsString()).arg(_anno->shape()->shapeInfo()));
        }

        dt::AnnoPolygon *AnnoGraphicsPolygon::annoPolygon() {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypePolygon) {
                return reinterpret_cast<dt::AnnoPolygon *>(_anno->shape());
            }
            return NULL;
        }

        const dt::AnnoPolygon *AnnoGraphicsPolygon::annoPolygon() const {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypePolygon) {
                return reinterpret_cast<dt::AnnoPolygon *>(_anno->shape());
            }
            return NULL;
        }

        void AnnoGraphicsPolygon::initControlPoints() {
            QPolygonF poly = polygon();
            for (int i = 0; i < poly.size(); ++i) {
                AnnoGraphicsControlPoint *cp = new AnnoGraphicsControlPoint(this, i);
                _cp.append(cp);
            }
            setControlPointsVisible(false);
            validateCpPos();
        }

        void AnnoGraphicsPolygon::validateCpPos() {
            dt::AnnoPolygon *poly = annoPolygon();
            for (int i = 0; i < poly->size(); ++i) {
                QPointF p = poly->at(i);
                moveControlPointTo(i, p.x(), p.y());
            }
        }

        void AnnoGraphicsPolygon::appendPolygonPoint(const QPointF &p) {
            dt::AnnoPolygon *poly = annoPolygon();
            if (poly != NULL) {
                prepareGeometryChange();
                poly->append(p);
                insertControlPoint(poly->size() - 1, new AnnoGraphicsControlPoint(this, poly->size() - 1));
                setPolygon(*poly);
                validateCpPos();
            } else {
                GlobalLogger::instance()->logError("AnnoGraphicsPolygon::appendPolygonPoint: poly == NULL");
            }
        }

        void AnnoGraphicsPolygon::setClosedDrawing(bool closed) {
            _drawClosed = closed;
        }

        void AnnoGraphicsPolygon::cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_POLY: cpMousePressEvent on CP %1").arg(index));
        }

        void AnnoGraphicsPolygon::cpMouseReleaseEvent(int index,
                QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_POLY: cpMouseReleaseEvent on CP %1").arg(index));
            setToolTip(QString("%1\n%2").arg(_anno->annoIdAsString()).arg(_anno->shape()->shapeInfo()));
        }

        void AnnoGraphicsPolygon::cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_POLY: cpMouseMoveEvent on CP %1").arg(index));
            qreal deltaX = event->scenePos().x() - event->lastScenePos().x();
            qreal deltaY = event->scenePos().y() - event->lastScenePos().y();
            QPointF delta(deltaX, deltaY);

            dt::AnnoPolygon *poly = annoPolygon();
            QPointF nPoint = poly->at(index);
            nPoint += delta;

            QRectF parRect = parentItem()->boundingRect();
            if (parRect.contains(nPoint)) {
                prepareGeometryChange();
                (*poly)[index] = nPoint;
                setPolygon(*poly);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(QString("%1\n%2").arg(_anno->annoIdAsString()).arg(_anno->shape()->shapeInfo()));
            }
        }

        void AnnoGraphicsPolygon::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: mousePressEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsPolygon::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: mouseReleaseEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsPolygon::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsPolygon::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: hoverEnterEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverEnterEvent(this, event);
            }
        }

        void AnnoGraphicsPolygon::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: hoverLeaveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverLeaveEvent(this, event);
            }
        }

        void AnnoGraphicsPolygon::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: hoverMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverMoveEvent(this, event);
            }
        }

        QVariant AnnoGraphicsPolygon::itemChange(GraphicsItemChange change,
                const QVariant &value) {
            if (change == QGraphicsItem::ItemSelectedChange) {
                ShapeConfig sc = GlobalConfig::instance()->getShapeConfig("polygon");
                if (value.toBool()) {
                    setControlPointsVisible(true);
                    setPen(sc.penSelected);
                    setBrush(sc.brushSelected);
                } else {
                    setControlPointsVisible(false);
                    setPen(sc.penNormal);
                    setBrush(sc.brushNormal);
                }
            }
            return QGraphicsPolygonItem::itemChange(change, value);
        }

        QGraphicsItem *AnnoGraphicsPolygon::graphicsItem() {
            return this;
        }

        void AnnoGraphicsPolygon::shapeMoveBy(qreal deltaX, qreal deltaY) {
            QPointF delta(deltaX, deltaY);
            dt::AnnoPolygon *poly = annoPolygon();
            QPolygonF tmpPoly = *poly; //TODO must this be mapped to parent?
            QRectF parRect = parentItem()->boundingRect();
            tmpPoly.translate(delta);

            bool validPos = true;
            for (int i = 0; i < tmpPoly.size(); ++i) {
                validPos = validPos && parRect.contains(tmpPoly[i]);
            }

            if (validPos) {
                prepareGeometryChange();
                *poly = tmpPoly;
                setPolygon(tmpPoly);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(QString("%1\n%2").arg(_anno->annoIdAsString()).arg(_anno->shape()->shapeInfo()));
            }
        }

        void AnnoGraphicsPolygon::shapeSizeBy(qreal facX, qreal facY) {
            //TODO implement this!
        }

        void AnnoGraphicsPolygon::paint(QPainter *painter,
                                        const QStyleOptionGraphicsItem *option, QWidget *widget) {
            painter->setPen(pen());
            painter->setBrush(brush());

            if(_drawClosed) {
                painter->drawPolygon(polygon());
            } else {
                painter->drawPolyline(polygon());
            }
        }

        void AnnoGraphicsPolygon::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: exMouseMoveEvent.");
            mouseMoveEvent(event);
        }

        void AnnoGraphicsPolygon::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: exMousePressEvent.");
            mousePressEvent(event);
        }

        void AnnoGraphicsPolygon::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: exMouseReleaseEvent.");
            mouseReleaseEvent(event);
        }

    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
