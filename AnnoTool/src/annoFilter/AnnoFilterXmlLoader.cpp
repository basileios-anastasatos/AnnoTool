#include "include/AnnoFilterXmlLoader.h"
#include "AllAnnoExceptions.h"
#include "AllAnnoFilterRules.h"
#include "AnnoProject.h"
#include "XmlHelper.h"
#include "AnnoFilter.h"

using anno::helper::XmlHelper;

#include <QXmlStreamReader>


namespace anno {
    namespace filter {

        AnnoFilterRule *AnnoFilterXmlLoader::loadRule(QXmlStreamReader &reader) throw(exc::XmlException *) {
            if(!reader.isStartElement()) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "any start element", reader.name().toString());
            }

            AnnoFilterRule *pRule = NULL;
            QString curName = reader.name().toString();

            if(AfrAnd::isXmlName(curName)) {
                pRule = AfrAnd::fromXml(reader);
            } else if (AfrOr::isXmlName(curName)) {
                pRule = AfrOr::fromXml(reader);
            } else if (AfrXor::isXmlName(curName)) {
                pRule = AfrXor::fromXml(reader);
            } else if (AfrNot::isXmlName(curName)) {
                pRule = AfrNot::fromXml(reader);
            } else if (AfrClass::isXmlName(curName)) {
                pRule = AfrClass::fromXml(reader);
            } else if (AfrAttributeValue::isXmlName(curName)) {
                pRule = AfrAttributeValue::fromXml(reader);
            } else if (AfrScore::isXmlName(curName)) {
                pRule = AfrScore::fromXml(reader);
            } else if (AfrSwitch::isXmlName(curName)) {
                pRule = AfrSwitch::fromXml(reader);
            }

            return pRule;
        }

        AnnoFilter *AnnoFilterXmlLoader::loadFilter(QXmlStreamReader &reader) throw(exc::XmlException *) {
            QString curName = reader.name().toString();

            if(!reader.isStartElement() || curName != AnnoFilter::XML_SINGLEFILTER) {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, AnnoFilter::XML_SINGLEFILTER, curName);
            }

            AnnoFilter *pFilter = AnnoFilter::fromXml(reader);

            return pFilter;
        }


    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
