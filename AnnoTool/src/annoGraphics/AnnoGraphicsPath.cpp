#include "AnnoGraphicsPath.h"
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
#include <QVariant>

namespace anno {
    namespace graphics {

        AnnoGraphicsPath::AnnoGraphicsPath(dt::Annotation *anno, QGraphicsItem *parent) :
            QGraphicsPathItem(parent), AnnoGraphicsShape(anno) {
            setupAppearance();
            _bIsForeground = true;
            _painterPath = NULL;
        }

        AnnoGraphicsPath::~AnnoGraphicsPath() {
            if (NULL != _painterPath) {
                delete _painterPath;
                _painterPath = NULL;
            }
        }

        void AnnoGraphicsPath::setupAppearance() {
            setFlag(QGraphicsItem::ItemIsSelectable);
            setFlag(QGraphicsItem::ItemIsFocusable);
            setVisible(true);
            setAcceptHoverEvents(true);
            setToolTip(_anno->annoInfo());
        }

        dt::AnnoPath *AnnoGraphicsPath::annoPath() {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypePath) {
                return reinterpret_cast<dt::AnnoPath *>(_anno->shape());
            }
            return NULL;
        }

        const dt::AnnoPath *AnnoGraphicsPath::annoPath() const {
            if (_anno != NULL && _anno->shape() != NULL && _anno->shape()->shapeType() == dt::ASTypePath) {
                return reinterpret_cast<dt::AnnoPath *>(_anno->shape());
            }
            return NULL;
        }

        void AnnoGraphicsPath::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent) {
            dt::Annotation *anno = relatedAnno();
            if(GlobalProjectManager::instance()->isAnnoSelected(anno)) {
                GlobalToolManager::instance()->triggerShapeContextMenu(relatedAnno());
            }
        }

        void AnnoGraphicsPath::mousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: mousePressEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mousePressEvent(this, event);
            }
        }

        void AnnoGraphicsPath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: mouseReleaseEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseReleaseEvent(this, event);
            }
        }

        void AnnoGraphicsPath::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: mouseDoubleClickEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseDoubleClickEvent(this, event);
            }
        }

        void AnnoGraphicsPath::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: mouseMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->mouseMoveEvent(this, event);
            }
        }

        void AnnoGraphicsPath::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: hoverEnterEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverEnterEvent(this, event);
            }
        }

        void AnnoGraphicsPath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: hoverLeaveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverLeaveEvent(this, event);
            }
        }

        void AnnoGraphicsPath::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: hoverMoveEvent.");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->hoverMoveEvent(this, event);
            }
        }

        void AnnoGraphicsPath::keyPressEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: keyPressEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyPressEvent(this, event);
            }
        }

        void AnnoGraphicsPath::keyReleaseEvent(QKeyEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: keyReleaseEvent");
            GlobalToolManager *tm = GlobalToolManager::instance();
            if (tm->hasTool()) {
                tm->curTool()->keyReleaseEvent(this, event);
            }
        }

        QVariant AnnoGraphicsPath::itemChange(GraphicsItemChange change,
                                              const QVariant &value) {
            if (change == QGraphicsItem::ItemSelectedChange) {
                if (value.toBool()) {
                    setVisible(true);
                } else {
                    setVisible(false);
                }
            }
            return QGraphicsPathItem::itemChange(change, value);
        }

        QGraphicsItem *AnnoGraphicsPath::graphicsItem() {
            return this;
        }

        void AnnoGraphicsPath::shapeMoveBy(qreal deltaX, qreal deltaY) {
        }

        void AnnoGraphicsPath::shapeSizeBy(qreal facX, qreal facY) {
            //TODO implement this!
        }

        dt::AnnoShapeType AnnoGraphicsPath::shapeType() const {
            return dt::ASTypePath;
        }

        void AnnoGraphicsPath::paint(QPainter *painter,
                                     const QStyleOptionGraphicsItem *option, QWidget *widget) {
            _shapeConfig.applyShapeConfig(painter, isSelected());

            if(_bIsForeground) {
                painter->setPen(QPen(QColor("cyan"), 3.0));
            } else {
                painter->setPen(QPen(QColor("blue"), 3.0));
            }

            painter->drawPath(path()/**_painterPath*/);
        }

        void AnnoGraphicsPath::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: exMouseMoveEvent.");
            mouseMoveEvent(event);
        }

        void AnnoGraphicsPath::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: exMousePressEvent.");
            mousePressEvent(event);
        }

        void AnnoGraphicsPath::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            GlobalLogger::instance()->logDebug("AG_Path: exMouseReleaseEvent.");
            mouseReleaseEvent(event);
        }

        void AnnoGraphicsPath::startPathFromPoint(const QPointF &pt) {
            if (NULL != _painterPath) {
                delete _painterPath;
                _painterPath = NULL;
            }
            _painterPath = new QPainterPath();
            _painterPath->moveTo(pt);
            setPath(*_painterPath);
        }

        void AnnoGraphicsPath::addPointToPath(const QPointF &pt) {
            _painterPath->lineTo(pt);
            setPath(*_painterPath);
        }

        QPainterPath *AnnoGraphicsPath::getPath() const {
            return _painterPath;
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
