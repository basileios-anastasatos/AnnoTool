#include "include/ColorFilterEntry.h"
#include "AnnoFilter.h"

namespace anno {

    namespace filter {

        ColorFilterEntry::ColorFilterEntry(AnnoFilter *filter) :
            _filter(filter) {

        }

        ColorFilterEntry::ColorFilterEntry(AnnoFilter *filter, graphics::VisualShapeConfig shapeConfig) :
            _filter(filter), _shapeConfig(shapeConfig) {

        }


        bool ColorFilterEntry::eval(const dt::Annotation *anno) const throw(exc::IllegalStateException *) {
            if(_filter != NULL && _filter->hasRule()) {
                return _filter->getFilterRule()->eval(anno);
            }
            return false;
        }

        graphics::VisualShapeConfig ColorFilterEntry::getColoring() const {
            return _shapeConfig;
        }

        void ColorFilterEntry::setColoring(const graphics::VisualShapeConfig &config) {
            _shapeConfig = config;
        }

        AnnoFilter *ColorFilterEntry::getFilter() const {
            return _filter;
        }


    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
