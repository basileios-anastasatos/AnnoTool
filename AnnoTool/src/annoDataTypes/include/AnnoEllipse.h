#ifndef ANNOELLIPSE_H_
#define ANNOELLIPSE_H_

#include <QRectF>
#include "AnnoShape.h"

namespace anno {
    namespace dt {

        class AnnoEllipse : public QRectF, public AnnoShape {
            public:
                AnnoEllipse();
                virtual ~AnnoEllipse();

            public:
                virtual AnnoShapeType shapeType() const;
                virtual QString shapeInfo() const;
                virtual QRectF boundingRect() const;

            public:
                virtual void print() const;

            public:
                void setRadiusX(qreal r);
                void setRadiusY(qreal r);
                void setRadi(qreal rx, qreal ry);

                qreal centerX() const;
                qreal centerY() const;
                qreal radiusX() const;
                qreal radiusY() const;

            public:
                AnnoEllipse &operator=(const QRectF &r);
                AnnoEllipse &operator=(const AnnoEllipse &r);

            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

    }

}

#endif /*ANNOELLIPSE_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
