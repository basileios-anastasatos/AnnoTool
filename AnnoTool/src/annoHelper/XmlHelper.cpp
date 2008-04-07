#include "include/XmlHelper.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

//namespace AnnoTool
namespace anno {
    //namespace Helper
    namespace helper {
        bool XmlHelper::skipToStartElement(QString tagName, QXmlStreamReader &reader)
        throw(XmlException *) {
            bool found = false;
            while (!reader.atEnd()) {
                if (reader.isStartElement() && reader.name() == tagName) {
                    found = true;
                    break;
                }
                reader.readNext();
            }

            if (reader.hasError()) {
                throw new XmlException(__FILE__, __LINE__, QString("An error occured while processing XML stream: %1").arg(reader.errorString()));
            }

            return found;
        }

        bool XmlHelper::skipToEndElement(QString tagName, QXmlStreamReader &reader)
        throw(XmlException *) {
            bool found = false;
            while (!reader.atEnd()) {
                if (reader.isEndElement() && reader.name() == tagName) {
                    found = true;
                    break;
                }
                reader.readNext();
            }

            if (reader.hasError()) {
                throw new XmlException(__FILE__, __LINE__, QString("An error occured while processing XML stream: %1").arg(reader.errorString()));
            }

            return found;
        }

        void XmlHelper::skipToNextStartElement(bool skipCur,
                                               QXmlStreamReader &reader) throw(XmlException *) {
            if (skipCur && !reader.atEnd()) {
                reader.readNext();
            }

            while (!reader.atEnd()) {
                if (reader.isStartElement()) {
                    break;
                }
                reader.readNext();
            }

            if (reader.hasError()) {
                throw new XmlException(__FILE__, __LINE__, QString("An error occured while processing XML stream: %1").arg(reader.errorString()));
            }
        }

        void XmlHelper::skipToNextEndElement(bool skipCur, QXmlStreamReader &reader)
        throw(XmlException *) {
            if (skipCur && !reader.atEnd()) {
                reader.readNext();
            }

            while (!reader.atEnd()) {
                if (reader.isEndElement()) {
                    break;
                }
                reader.readNext();
            }

            if (reader.hasError()) {
                throw new XmlException(__FILE__, __LINE__, QString("An error occured while processing XML stream: %1").arg(reader.errorString()));
            }
        }

        QString XmlHelper::uuidAsString(const QUuid &uuid) {
            QString str = uuid.toString();
            str.remove(0, 1);
            str.remove(str.length() - 1, 1);
            return str;
        }

    } //end namespace helper
} //end namespace anno

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
