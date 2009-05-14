#include "include/VisualShapeConfig.h"
#include "XmlHelper.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QPainter>

#include "importGlobals.h"

using anno::helper::XmlHelper;

namespace anno {

    namespace graphics {

        VisualShapeConfig::VisualShapeConfig() :
            _borderNormal(QColor(230, 230, 230)), _borderSelected(QColor(255, 255, 255)),
    /* MA: better colors in VNC */
            //_fillNormal(QColor(255, 255, 255, 0)), _fillSelected(QColor(255, 255, 255, 30))
            _fillNormal(QColor(255, 255, 255, 0)), _fillSelected(QColor(255, 255, 255, 0)) {
            _borderNormal.setWidth(1);
            _borderSelected.setWidth(2);
        }

        VisualShapeConfig::VisualShapeConfig(const QColor &border, const QColor &fill, int borderWidth) :
            _borderNormal(border), _fillNormal(fill) {
            _borderNormal.setWidth(borderWidth);
            autoAdjustSel();
        }

        VisualShapeConfig::VisualShapeConfig(const QColor &borderNormal, const QColor &borderSelected, const QColor &fillNormal, const QColor &fillSelected, int borderWidthNormal, int borderWidthSelected) :
            _borderNormal(borderNormal), _borderSelected(borderSelected),
            _fillNormal(fillNormal), _fillSelected(fillSelected) {
            _borderNormal.setWidth(borderWidthNormal);
            _borderSelected.setWidth(borderWidthSelected);
        }

        void VisualShapeConfig::autoAdjustSel() {
            _borderSelected.setColor(_borderNormal.color().lighter(125));
            _borderSelected.setWidth(_borderNormal.width() + 1);

            QColor tmp(_fillNormal.color().lighter(120));
            tmp.setAlphaF(_fillNormal.color().alphaF() * 1.1);
            _fillSelected.setColor(tmp);
        }

        void VisualShapeConfig::applyShapeConfig(QPainter *painter, bool selected) {
            if(selected) {
                painter->setPen(_borderSelected);
                painter->setBrush(_fillSelected);
            } else {
                painter->setPen(_borderNormal);
                painter->setBrush(_fillNormal);
            }
        }

        void VisualShapeConfig::toXml(QXmlStreamWriter &writer) const {
            writer.writeStartElement("visualShapeConfig");
            writer.writeStartElement("normalState");
            writer.writeTextElement("borderWidth", QString::number(_borderNormal.width(), 10));
            writer.writeTextElement("borderColor", XmlHelper::colorAsArgbString(_borderNormal.color()));
            writer.writeTextElement("fillColor", XmlHelper::colorAsArgbString(_fillNormal.color()));
            writer.writeEndElement();
            writer.writeStartElement("selectedState");
            writer.writeTextElement("borderWidth", QString::number(_borderSelected.width(), 10));
            writer.writeTextElement("borderColor", XmlHelper::colorAsArgbString(_borderSelected.color()));
            writer.writeTextElement("fillColor", XmlHelper::colorAsArgbString(_fillSelected.color()));
            writer.writeEndElement();
            writer.writeEndElement();
        }

        void VisualShapeConfig::loadState(QXmlStreamReader &reader) throw(exc::XmlException *) {
            XmlHelper::skipToNextStartElement(true, reader);
            if(reader.isStartElement()) {
                if(reader.name() == "normalState") {
                    if(!XmlHelper::skipToStartElement("borderWidth", reader)) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, expected <borderWdith> tag.");
                    }
                    QString strWidth = reader.readElementText();
                    bool ok = false;
                    int width = strWidth.toInt(&ok, 10);
                    if(!ok || width < 0) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid border width entry.");
                    }
                    _borderNormal.setWidth(width);


                    if(!XmlHelper::skipToStartElement("borderColor", reader)) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, expected <borderColor> tag.");
                    }
                    _borderNormal.setColor(XmlHelper::argbStringToColor(reader.readElementText()));

                    if(!XmlHelper::skipToStartElement("fillColor", reader)) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, expected <fillColor> tag.");
                    }
                    _fillNormal.setColor(XmlHelper::argbStringToColor(reader.readElementText()));

                    XmlHelper::skipToEndElement("normalState", reader);
                } else if(reader.name() == "selectedState") {
                    if(!XmlHelper::skipToStartElement("borderWidth", reader)) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, expected <borderWidth> tag.");
                    }
                    QString strWidth = reader.readElementText();
                    bool ok = false;
                    int width = strWidth.toInt(&ok, 10);
                    if(!ok || width < 0) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid border width entry.");
                    }
                    _borderSelected.setWidth(width);


                    if(!XmlHelper::skipToStartElement("borderColor", reader)) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, expected <borderColor> tag.");
                    }
                    _borderSelected.setColor(XmlHelper::argbStringToColor(reader.readElementText()));

                    if(!XmlHelper::skipToStartElement("fillColor", reader)) {
                        throw new exc::XmlFormatException(__FILE__, __LINE__, "Invalid XML structure, expected <fillColor> tag.");
                    }
                    _fillSelected.setColor(XmlHelper::argbStringToColor(reader.readElementText()));

                    XmlHelper::skipToEndElement("selectedState", reader);
                } else {
                    throw new exc::XmlFormatException(__FILE__, __LINE__, QString("Expected tag <normalState> or <selectedState> but found <%1>.").arg(reader.name().toString()));
                }
            } else {
                throw new exc::XmlFormatException(__FILE__, __LINE__, "Expected a start tag.");
            }
        }


        void VisualShapeConfig::loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            if (!reader.isStartElement() || reader.name() != "visualShapeConfig") {
                throw XmlHelper::genExpStreamPos(__FILE__, __LINE__, "visualShapeConfig", reader.name().toString());
            }

            loadState(reader);
            loadState(reader);
            XmlHelper::skipToEndElement("visualShapeConfig", reader);
            reader.readNext();
        }

        VisualShapeConfig VisualShapeConfig::fromXml(QXmlStreamReader &reader) throw(exc::XmlException *) {
            VisualShapeConfig tmp;
            tmp.loadFromXml(reader);
            return tmp;
        }


    }

}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
