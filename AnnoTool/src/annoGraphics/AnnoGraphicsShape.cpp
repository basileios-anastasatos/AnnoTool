#include "include/AnnoGraphicsShape.h"
#include "AnnoGraphicsPixmap.h"

#include "GlobalLogger.h"
using ::logging::GlobalLogger;

namespace anno {
    namespace graphics {

        AnnoGraphicsShape::AnnoGraphicsShape(dt::Annotation *anno, AnnoGraphicsPixmap *img) {
            _anno = anno;
            _parentImage = img;
        }

        AnnoGraphicsShape::~AnnoGraphicsShape() {
        }

        dt::Annotation *AnnoGraphicsShape::relatedAnno() {
            return _anno;
        }

        AnnoGraphicsPixmap *AnnoGraphicsShape::parentImage() {
            return _parentImage;
        }

        void AnnoGraphicsShape::setParentImage(AnnoGraphicsPixmap *img) {
            _parentImage = img;
        }

        void AnnoGraphicsShape::initControlPoints() {
            //nothing to be done here!
        }

        void AnnoGraphicsShape::validateCpIndices() {
            GlobalLogger::instance()->logDebug("AG_SH: validating CP indices.");
            int size = _cp.size();
            for (int i = 0; i < size; ++i) {
                _cp[i]->setIndex(i);
            }
        }

        void AnnoGraphicsShape::insertControlPoint(int index, AnnoGraphicsControlPoint *cp) {
            GlobalLogger::instance()->logDebug(QString("AG_SH: inserting CP at %1.").arg(index));
            _cp.insert(index, cp);
            validateCpIndices();
            cp->setParentItem(graphicsItem());
        }

        void AnnoGraphicsShape::removeControlPoint(int index) {
            if (index >= 0 && index < _cp.size()) {
                GlobalLogger::instance()->logDebug(QString("AG_SH: removing CP from %1.").arg(index));
                AnnoGraphicsControlPoint *cp = _cp[index];
                cp->setParentItem(NULL);
                _cp.removeAt(index);
                delete cp;
                validateCpIndices();
            }
        }

        void AnnoGraphicsShape::moveControlPoint(int index, qreal x, qreal y) {
            if (index >= 0 && index < _cp.size()) {
                GlobalLogger::instance()->logDebug(QString("AG_SH: moving CP %1.").arg(index));
                AnnoGraphicsControlPoint *cp = _cp[index];
                cp->moveBy(x, y);
            }
        }

        void AnnoGraphicsShape::moveControlPointTo(int index, qreal x, qreal y) {
            if (index >= 0 && index < _cp.size()) {
                GlobalLogger::instance()->logDebug(QString("AG_SH: moving CP %1 to [%2;%3].").arg(index).arg(x).arg(y));
                AnnoGraphicsControlPoint *cp = _cp[index];
                QRectF rect = cp->rect();
                cp->setPos(x - (rect.width() / 2.0), y - (rect.height() / 2));
            }
        }

        void AnnoGraphicsShape::setControlPointsVisible(bool vis) {
            GlobalLogger::instance()->logDebug(QString("AG_SH: setting CPs to %1.").arg(vis ? "visible" : "invisible"));
            int size = _cp.size();
            for (int i = 0; i < size; ++i) {
                _cp[i]->setVisible(vis);
            }
        }

        void AnnoGraphicsShape::cpMousePressEvent(int index, QGraphicsSceneMouseEvent *event) {
            //nothing to be done here!
        }

        void AnnoGraphicsShape::cpMouseReleaseEvent(int index, QGraphicsSceneMouseEvent *event) {
            //nothing to be done here!
        }

        void AnnoGraphicsShape::cpMouseMoveEvent(int index, QGraphicsSceneMouseEvent *event) {
            //nothing to be done here!
        }

        void AnnoGraphicsShape::exMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
            //nothing to be done here!
        }

        void AnnoGraphicsShape::exMousePressEvent(QGraphicsSceneMouseEvent *event) {
            //nothing to be done here!
        }

        void AnnoGraphicsShape::exMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
            //nothing to be done here!
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
