#ifndef XMLHELPER_H_
#define XMLHELPER_H_

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AllAnnoExceptions.h"

//namespace AnnoTool
namespace anno {
    //namespace Helper
    namespace helper {
        using namespace ::anno::exc;
        class XmlHelper {
            public:
                static bool
                skipToStartElement(QString tagName, QXmlStreamReader &reader)
                throw(XmlException *);
                static bool skipToEndElement(QString tagName, QXmlStreamReader &reader)
                throw(XmlException *);

                static void skipToNextStartElement(bool skipCur, QXmlStreamReader &reader)
                throw(XmlException *);
                static void skipToNextEndElement(bool skipCur, QXmlStreamReader &reader)
                throw(XmlException *);
        };

    } //end namespace helper
} //end namespace anno
#endif /*XMLHELPER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
