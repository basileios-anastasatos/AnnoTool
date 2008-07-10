#ifndef XMLHELPER_H_
#define XMLHELPER_H_

#include <QString>
#include <QUuid>
#include "AllAnnoExceptions.h"

class QXmlStreamReader;
class QXmlStreamWriter;
class QPointF;
class QSizeF;
class QColor;

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

                static void skipToNextStartElement(bool skipCur,
                                                   QXmlStreamReader &reader) throw(XmlException *);
                static void
                skipToNextEndElement(bool skipCur, QXmlStreamReader &reader)
                throw(XmlException *);

            public:
                static void writeXmlPoint(QXmlStreamWriter &writer, qreal x, qreal y);
                static void writeXmlPoint(QXmlStreamWriter &writer, const QString &tag, qreal x, qreal y);
                static void writeXmlSize(QXmlStreamWriter &writer, qreal width,
                                         qreal height);
                static QPointF readXmlPoint(QXmlStreamReader &reader) throw(XmlException *);
                static QSizeF readXmlSize(QXmlStreamReader &reader) throw(XmlException *);

            public:
                static XmlFormatException *genExpFormatExpected(const char *file,
                        int line, const QString &expected, const QString &actual);
                static XmlFormatException *genExpFormatAttr(const char *file, int line,
                        const QString &attribute, const QString &value);
                static XmlException *genExpStreamPos(const char *file, int line,
                                                     const QString &expected, const QString &actual);

            public:
                static QString uuidAsString(const QUuid &uuid);
                static QString colorAsArgbString(const QColor &color);
                static QColor argbStringToColor(const QString &str);
        };

    } //end namespace helper
} //end namespace anno
#endif /*XMLHELPER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
