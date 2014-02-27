#include "include/ColorFilterEntry.h"
#include "AnnoFilter.h"
#include "AnnoFilterManager.h"
#include "GlobalProjectManager.h"
#include "XmlHelper.h"

using anno::helper::XmlHelper;

namespace anno {

    namespace filter {
        const QString ColorFilterEntry::XML_SINGLECOLORRULE("colorRule");
        const QString ColorFilterEntry::XML_FILTERNAME("filterName");

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

        void ColorFilterEntry::toXml(QXmlStreamWriter &writer) const {
            writer.writeStartElement(XML_SINGLECOLORRULE);
            writer.writeAttribute(XML_FILTERNAME, getFilter()->getName());
            getColoring().toXml(writer);
            writer.writeEndElement();
        }

        ColorFilterEntry *ColorFilterEntry::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            ColorFilterEntry *c = new ColorFilterEntry(0);
            c->loadFromXml(reader);
            return c;
        }

        void ColorFilterEntry::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            if (!reader.isStartElement() || reader.name() != XML_SINGLECOLORRULE) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, XML_SINGLECOLORRULE, reader.name().toString());
            }
            const QString filterName = reader.attributes().value(XML_FILTERNAME).toString();
            const filter::AnnoFilterManager *filterMan = GlobalProjectManager::instance()->filterMan();
            _filter = 0;
            if (filterMan) {
                _filter = GlobalProjectManager::instance()->filterMan()->getFilter(filterName);
            }
            if (!_filter) {
                throw new exc::NoSuchElementException(__FILE__, __LINE__, "Filter %1 not defined.");
            }

            XmlHelper::skipToNextStartElement(true, reader);
            setColoring(graphics::VisualShapeConfig::fromXml(reader));

            XmlHelper::skipToEndElement(XML_SINGLECOLORRULE, reader);
            reader.readNext();
        }
    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
