#ifndef VISUALSHAPECONFIG_H_
#define VISUALSHAPECONFIG_H_

#include <QPen>
#include <QBrush>
#include <QColor>
#include "AllAnnoExceptions.h"

class QXmlStreamReader;
class QXmlStreamWriter;
class QPainter;

namespace anno {
    namespace graphics {

        class VisualShapeConfig {
            private:
                QPen _borderNormal;
                QPen _borderSelected;
                QBrush _fillNormal;
                QBrush _fillSelected;

            private:
                void loadState(QXmlStreamReader &reader) throw(exc::XmlException *);

            public:
                VisualShapeConfig();
                VisualShapeConfig(const QColor &border, const QColor &fill, int borderWidth);
                VisualShapeConfig(const QColor &borderNormal, const QColor &borderSelected, const QColor &fillNormal, const QColor &fillSelected, int borderWidthNormal, int borderWidthSelected);

            public:
                void autoAdjustSel();

            public:
                QColor getBorderColor() const;
                QColor getBorderColorSel() const;
                QColor getFillColor() const;
                QColor getFillColorSel() const;
                int getBorderWidth() const;
                int getBorderWidthSel() const;

                QPen getBorderPen() const;
                QPen getBorderPenSel() const;
                QBrush getFill() const;
                QBrush getFillSel() const;

            public:
                void setBorderColor(const QColor &col);
                void setBorderColorSel(const QColor &col);
                void setFillColor(const QColor &col);
                void setFillColorSel(const QColor &col);
                void setBorderWidth(int width);
                void setBorderWidthSel(int width);

            public:
                void applyShapeConfig(QPainter *painter, bool selected);

                // Xml Interface
            public:
                void toXml(QXmlStreamWriter &writer) const;
                void loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
                static VisualShapeConfig fromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
        };


        // Inlineing
        // -------------------------------------------------------------------------------------------------
        inline QColor VisualShapeConfig::getBorderColor() const {
            return _borderNormal.color();
        }

        inline QColor VisualShapeConfig::getBorderColorSel() const {
            return _borderSelected.color();
        }

        inline QColor VisualShapeConfig::getFillColor() const {
            return _fillNormal.color();
        }

        inline QColor VisualShapeConfig::getFillColorSel() const {
            return _fillSelected.color();
        }

        inline int VisualShapeConfig::getBorderWidth() const {
            return _borderNormal.width();
        }

        inline int VisualShapeConfig::getBorderWidthSel() const {
            return _borderSelected.width();
        }

        inline QPen VisualShapeConfig::getBorderPen() const {
            return _borderNormal;
        }

        inline QPen VisualShapeConfig::getBorderPenSel() const {
            return _borderSelected;
        }

        inline QBrush VisualShapeConfig::getFill() const {
            return _fillNormal;
        }

        inline QBrush VisualShapeConfig::getFillSel() const {
            return _fillSelected;
        }

        inline void VisualShapeConfig::setBorderColor(const QColor &col) {
            _borderNormal.setColor(col);
        }

        inline void VisualShapeConfig::setBorderColorSel(const QColor &col) {
            _borderSelected.setColor(col);
        }

        inline void VisualShapeConfig::setFillColor(const QColor &col) {
            _fillNormal.setColor(col);
        }

        inline void VisualShapeConfig::setFillColorSel(const QColor &col) {
            _fillSelected.setColor(col);
        }

        inline void VisualShapeConfig::setBorderWidth(int width) {
            _borderNormal.setWidth(width);
        }

        inline void VisualShapeConfig::setBorderWidthSel(int width) {
            _borderSelected.setWidth(width);
        }


        // -------------------------------------------------------------------------------------------------

    }
}

#endif /* VISUALSHAPECONFIG_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
