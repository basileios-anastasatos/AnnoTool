#ifndef AFRSHAPE_H_
#define AFRSHAPE_H_ AFRSHAPE_H_

#include "AnnoFilterRuleAtom.h"
#include "AnnoShape.h"
#include <QMap>

namespace anno {
    namespace filter {

        class AfrShape: public AnnoFilterRuleAtom {
            private:
                static const QString XML_NAME;

            private:
                QString _shape;

            private:
                AfrShape();

            public:
                AfrShape(const QString &shape, bool autoDelete = true);
                virtual ~AfrShape();

                // Shape specific methods
            public:
                static bool isXmlName(const QString &str);
                static QString xmlName();
                static AfrShape *fromXml(QXmlStreamReader &reader) throw(exc::XmlException *);
                QString getShape() const;

                // inherited interface stuff
                // ------------------------------------------------------------------------------------
                // General interface
            public:
                virtual bool isValid() const;
                virtual QString toString() const;

                // XML interface
            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(exc::XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(exc::XmlException *);

                // Filtering interface
            public:
                virtual bool evalInternal(const dt::Annotation *anno) const throw(exc::IllegalStateException *);
                // ------------------------------------------------------------------------------------

            private:
                static const bool dummy;
                static QMap<QString, anno::dt::AnnoShapeType> shapeMap;
                static bool initShapeMap();
        };

        // Inlining
        // ------------------------------------------------------------------------------------
        inline bool AfrShape::isXmlName(const QString &str) {
            return (QString::compare(str, XML_NAME, Qt::CaseInsensitive) == 0);
        }

        inline QString AfrShape::xmlName() {
            return XML_NAME;
        }

        inline QString AfrShape::getShape() const {
            return _shape;
        }
        // ------------------------------------------------------------------------------------


    }
}

#endif /* AFRSHAPE_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
