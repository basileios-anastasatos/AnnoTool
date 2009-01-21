#ifndef COLORFILTERENTRY_H_
#define COLORFILTERENTRY_H_

#include "annoTypesForward.h"
#include "VisualShapeConfig.h"

namespace anno {
    namespace filter {
        class AnnoFilter;

        class ColorFilterEntry {
            private:
                AnnoFilter *_filter;
                graphics::VisualShapeConfig _shapeConfig;

            public:
                ColorFilterEntry(AnnoFilter *filter);
                ColorFilterEntry(AnnoFilter *filter, graphics::VisualShapeConfig shapeConfig);

            public:
                bool eval(const dt::Annotation *anno) const throw(exc::IllegalStateException *);

                graphics::VisualShapeConfig getColoring() const;
                void setColoring(const graphics::VisualShapeConfig &config);

                AnnoFilter *getFilter() const;
        };

    }
}

#endif /* COLORFILTERENTRY_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
