#include "include/AnnoGraphicsShape.h"

namespace anno {
    namespace graphics {

        AnnoGraphicsShape::AnnoGraphicsShape(dt::Annotation *anno) {
            _anno = anno;
        }

        AnnoGraphicsShape::~AnnoGraphicsShape() {
        }

        dt::Annotation *AnnoGraphicsShape::relatedAnno() {
            return _anno;
        }

        void AnnoGraphicsShape::initControlPoints() {
            //nothing to be done here!
        }

        void AnnoGraphicsShape::validateCpIndices() {
            int size = _cp.size();
            for (int i = 0; i < size; ++i) {
                _cp[i]->setIndex(i);
            }
        }

        void AnnoGraphicsShape::insertControlPoint(int index,
                AnnoGraphicsControlPoint *cp) {
            _cp.insert(index, cp);
            validateCpIndices();
            cp->setParentItem(graphicsItem());
        }

        void AnnoGraphicsShape::removeControlPoint(int index) {
            if (index >= 0 && index < _cp.size()) {
                AnnoGraphicsControlPoint *cp = _cp[index];
                cp->setParentItem(NULL);
                _cp.removeAt(index);
                delete cp;
                validateCpIndices();
            }
        }

        void AnnoGraphicsShape::moveControlPoint(int index, qreal x, qreal y) {
            if (index >= 0 && index < _cp.size()) {
                AnnoGraphicsControlPoint *cp = _cp[index];
                cp->moveBy(x, y);
            }
        }

        void AnnoGraphicsShape::moveControlPointTo(int index, qreal x, qreal y) {
            if (index >= 0 && index < _cp.size()) {
                AnnoGraphicsControlPoint *cp = _cp[index];
                cp->setPos(x, y);
                //TODO check if this is right because of the many coord-sys!
            }
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
