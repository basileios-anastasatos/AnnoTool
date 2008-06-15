#include "include/XmlHelper.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QPointF>
#include <QSizeF>

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

        void XmlHelper::skipToNextStartElement(bool skipCur, QXmlStreamReader &reader)
        throw(XmlException *) {
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

        void XmlHelper::writeXmlPoint(QXmlStreamWriter &writer, qreal x, qreal y) {
            writer.writeEmptyElement("point");
            writer.writeAttribute("x", QString::number(x, 'f', 2));
            writer.writeAttribute("y", QString::number(y, 'f', 2));
        }

        void XmlHelper::writeXmlPoint(QXmlStreamWriter &writer, const QString &tag, qreal x, qreal y) {
            writer.writeEmptyElement(tag);
            writer.writeAttribute("x", QString::number(x, 'f', 2));
            writer.writeAttribute("y", QString::number(y, 'f', 2));
        }

        void XmlHelper::writeXmlSize(QXmlStreamWriter &writer, qreal width, qreal height) {
            writer.writeEmptyElement("size");
            writer.writeAttribute("w", QString::number(width, 'f', 2));
            writer.writeAttribute("h", QString::number(height, 'f', 2));
        }

        QPointF XmlHelper::readXmlPoint(QXmlStreamReader &reader) throw(XmlException *) {
            QString strX = reader.attributes().value("x").toString();
            QString strY = reader.attributes().value("y").toString();
            if (strX.isEmpty() || strY.isEmpty()) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid Xml-Point format.");
            }

            bool ok = false;
            double x = strX.toDouble(&ok);
            double y = strY.toDouble(&ok);
            if (!ok) {
                throw new XmlException(__FILE__, __LINE__, "Could not parse Xml-Point data.");
            }

            return QPointF(x, y);
        }

        QSizeF XmlHelper::readXmlSize(QXmlStreamReader &reader) throw(XmlException *) {
            QString strW = reader.attributes().value("w").toString();
            QString strH = reader.attributes().value("h").toString();
            if (strW.isEmpty() || strH.isEmpty()) {
                throw new XmlFormatException(__FILE__, __LINE__, "Invalid Xml-Size format.");
            }

            bool ok = false;
            double w = strW.toDouble(&ok);
            double h = strH.toDouble(&ok);
            if (!ok) {
                throw new XmlException(__FILE__, __LINE__, "Could not parse Xml-Size data.");
            }

            return QSizeF(w, h);
        }

        XmlFormatException *XmlHelper::genExpFormatExpected(const char *file, int line,
                const QString &expected, const QString &actual) {
            QString msg("Invalid Xml format. Was expecting tag <%1> but got <%2>");
            msg = msg.arg(expected, actual);
            return new XmlFormatException(file, line, msg);
        }

        XmlFormatException *XmlHelper::genExpFormatAttr(const char *file, int line,
                const QString &attribute, const QString &value) {
            QString msg("Invalid Xml-Attribute. Attribute [%1] had value [%2]");
            msg = msg.arg(attribute, value);
            return new XmlFormatException(file, line, msg);
        }

        XmlException *XmlHelper::genExpStreamPos(const char *file, int line,
                const QString &expected, const QString &actual) {
            QString msg("Illegal Xml stream position. Was expecting tag <%1> but got <%2>");
            msg = msg.arg(expected, actual);
            return new XmlFormatException(file, line, msg);
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
