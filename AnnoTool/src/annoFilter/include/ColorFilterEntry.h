#ifndef COLORFILTERENTRY_H_
#define COLORFILTERENTRY_H_

#include "annoTypesForward.h"
#include "VisualShapeConfig.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace anno {
    namespace filter {
        class AnnoFilter;

        class ColorFilterEntry {
                // some useful constants
            public:
                static const QString XML_SINGLECOLORRULE;
                static const QString XML_FILTERNAME;

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

                // Xml Interface
            public:
                void toXml(QXmlStreamWriter &writer) const;
                void loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
                static ColorFilterEntry *fromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
        };

    }
}

#endif /* COLORFILTERENTRY_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
