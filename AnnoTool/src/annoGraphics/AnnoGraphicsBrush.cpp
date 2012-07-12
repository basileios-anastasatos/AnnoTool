#include "AnnoGraphicsBrush.h"
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

        AnnoGraphicsBrush::AnnoGraphicsBrush(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsPolygonItem(parent), AnnoGraphicsShape(anno), _drawClosed(true) {
            setupAppearance();
            setPolygon(*annoPolygon());
            initControlPoints();
        }

        AnnoGraphicsBrush::~AnnoGraphicsBrush() {
        }

        void AnnoGraphicsBrush::setupAppearance() {
            setFlag(QGraphicsItem::ItemIsSelectable);
            setFlag(QGraphicsItem::ItemIsFocusable);
            setVisible(true);
            setAcceptsHoverEvents(true);
            setToolTip(_anno->annoInfo());
        }

        dt::AnnoBrush *AnnoGraphicsBrush::annoPolygon() {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypePolygon) {
                return reinterpret_cast<dt::AnnoBrush *>(_anno->shape());
            }
            return NULL;
        }

        const dt::AnnoBrush *AnnoGraphicsBrush::annoPolygon() const {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypePolygon) {
                return reinterpret_cast<dt::AnnoBrush *>(_anno->shape());
            }
            return NULL;
        }

        void AnnoGraphicsBrush::initControlPoints() {
            QPolygonF poly = polygon();
            for (int i = 0; i < poly.size(); ++i) {
                AnnoGraphicsControlPoint *cp = new AnnoGraphicsControlPoint(this, i);
                _cp.append(cp);
            }
            setControlPointsVisible(false);
            validateCpPos();
        }

        void AnnoGraphicsBrush::validateCpPos() {
            dt::AnnoBrush *poly = annoPolygon();
            for (int i = 0; i < poly->size(); ++i) {
                QPointF p = poly->at(i);
                moveControlPointTo(i, p.x(), p.y());
            }
        }

        void AnnoGraphicsBrush::appendPolygonPoint(const QPointF &p) {
            dt::AnnoBrush *poly = annoPolygon();
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

        void AnnoGraphicsBrush::insertPolygonPoint(const QPointF &p) {
            dt::AnnoBrush *poly = annoPolygon();
            if (poly != NULL && !p.isNull()) {
                int insertIndex = -1;
                int size = poly->size();

                for(int i = 1; i <= size; ++i) {
                    int idxFirst = i - 1;
                    int idxLast = i % size;
                    QPointF first = poly->at(idxFirst);
                    QPointF last = poly->at(idxLast);
                    QLineF mainLine(first, last);
                    QLineF norm = mainLine.normalVector().unitVector();
                    norm.setLength(4.0);
                    QLineF firstLine(mainLine);
                    QLineF secondLine(mainLine);
                    firstLine.translate(norm.dx(), norm.dy());
                    secondLine.translate(-norm.dx(), -norm.dy());

                    QPainterPath path;
                    path.moveTo(firstLine.p1());
                    path.lineTo(firstLine.p2());
                    path.lineTo(secondLine.p2());
                    path.lineTo(secondLine.p1());
                    path.closeSubpath();

                    if(path.contains(p)) {
                        insertIndex = idxLast;
                        break;
                    }
                }

                if(insertIndex >= 0) {
                    prepareGeometryChange();
                    poly->insert(insertIndex, p);
                    insertControlPoint(insertIndex, new AnnoGraphicsControlPoint(this, insertIndex));
                    setPolygon(*poly);
                    validateCpIndices();
                    validateCpPos();
                }
            } else {
                GlobalLogger::instance()->logError("AnnoGraphicsPolygon::insertPolygonPoint: poly == NULL");
            }
        }

        void AnnoGraphicsBrush::removePolygonPoint(int index) {
            dt::AnnoBrush *poly = annoPolygon();
            prepareGeometryChange();
            poly->remove(index);
            removeControlPoint(index);
            setPolygon(*poly);
            validateCpIndices();
            validateCpPos();
        }

        void AnnoGraphicsBrush::setClosedDrawing(bool closed) {
            _drawClosed = closed;
        }

        void AnnoGraphicsBrush::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent) {
            dt::Annotation *anno = relatedAnno();
            if(GlobalProjectManager::instance()->isAnnoSelected(anno)) {
                GlobalToolManager::instance()->triggerShapeContextMenu(relatedAnno());
            }
        }

        void AnnoGraphicsBrush::cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_POLY: cpMousePressEvent on CP %1").arg(index));
        }

        void AnnoGraphicsBrush::cpMouseReleaseEvent(int index,
                QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_POLY: cpMouseReleaseEvent on CP %1").arg(index));
            setToolTip(_anno->annoInfo());
        }

        void AnnoGraphicsBrush::cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug(QString("AG_POLY: cpMouseMoveEvent on CP %1").arg(index));
            qreal deltaX = event->scenePos().x() - event->lastScenePos().x();
            qreal deltaY = event->scenePos().y() - event->lastScenePos().y();
            QPointF delta(deltaX, deltaY);

            dt::AnnoBrush *poly = annoPolygon();
            QPointF nPoint = poly->at(index);
            nPoint += delta;

            QRectF parRect = parentItem()->boundingRect();
            if (parRect.contains(nPoint)) {
                prepareGeometryChange();
                (*poly)[index] = nPoint;
                setPolygon(*poly);
                validateCpPos();
                _anno->setModified(true);
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsBrush::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: mousePressEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsBrush::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: mouseReleaseEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsBrush::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: mouseDoubleClickEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseDoubleClickEvent(this, event);
            }
        }

        void AnnoGraphicsBrush::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsBrush::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: hoverEnterEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverEnterEvent(this, event);
            }
        }

        void AnnoGraphicsBrush::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: hoverLeaveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverLeaveEvent(this, event);
            }
        }

        void AnnoGraphicsBrush::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: hoverMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverMoveEvent(this, event);
            }
        }

        void AnnoGraphicsBrush::keyPressEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: keyPressEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyPressEvent(this, event);
            }
        }

        void AnnoGraphicsBrush::keyReleaseEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: keyReleaseEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyReleaseEvent(this, event);
            }
        }

        QVariant AnnoGraphicsBrush::itemChange(GraphicsItemChange change,
                                               const QVariant &value) {
            if (change == QGraphicsItem::ItemSelectedChange) {
                if (value.toBool()) {
                    setControlPointsVisible(true);
                } else {
                    setControlPointsVisible(false);
                }
            }
            return QGraphicsPolygonItem::itemChange(change, value);
        }

        QGraphicsItem *AnnoGraphicsBrush::graphicsItem() {
            return this;
        }

        void AnnoGraphicsBrush::shapeMoveBy(qreal deltaX, qreal deltaY) {
            QPointF delta(deltaX, deltaY);
            dt::AnnoBrush *poly = annoPolygon();
            QPolygonF tmpPoly = *poly;
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
                setToolTip(_anno->annoInfo());
            }
        }

        void AnnoGraphicsBrush::shapeSizeBy(qreal facX, qreal facY) {
            //TODO implement this!
        }

        dt::AnnoShapeType AnnoGraphicsBrush::shapeType() const {
            return dt::ASTypePolygon;
        }

        void AnnoGraphicsBrush::paint(QPainter *painter,
                                      const QStyleOptionGraphicsItem *option, QWidget *widget) {
            _shapeConfig.applyShapeConfig(painter, isSelected());

            if(_drawClosed) {
                painter->drawPolygon(polygon());
            } else {
                painter->drawPolyline(polygon());
            }

//			if(!_paths.isEmpty())
//			{
//				GlobalLogger::instance()->logDebug(">>> Painting paths!");
//				QPen ppen(Qt::SolidLine);
//				ppen.setWidth(1);
//				ppen.setColor(QColor(0, 240, 0, 255));
//				QBrush pbrush(Qt::SolidPattern);
//				pbrush.setColor(QColor(255, 255, 255, 0));
//				painter->setPen(ppen);
//				painter->setBrush(pbrush);
//
//				QListIterator<QPainterPath> i(_paths);
//				while(i.hasNext())
//				{
//					QPainterPath p = i.next();
//					painter->drawPath(p);
//				}
//			}
        }

        void AnnoGraphicsBrush::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: exMouseMoveEvent.");
            mouseMoveEvent(event);
        }

        void AnnoGraphicsBrush::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: exMousePressEvent.");
            mousePressEvent(event);
        }

        void AnnoGraphicsBrush::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_POLY: exMouseReleaseEvent.");
            mouseReleaseEvent(event);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
