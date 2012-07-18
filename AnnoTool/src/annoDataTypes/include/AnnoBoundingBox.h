#pragma once

#include <QRectF>
#include <QImage>
#include "AnnoShape.h"

namespace anno {
    namespace dt {

        class AnnoBoundingBox : public QRectF, public AnnoShape {
            public:
                AnnoBoundingBox();
                virtual ~AnnoBoundingBox();

            public:
                virtual AnnoShapeType shapeType() const;
                virtual QString shapeInfo() const;
                virtual QRectF boundingRect() const;

            public:
                virtual void print() const;

            public:
                AnnoBoundingBox &operator=(const QRectF &r);
                AnnoBoundingBox &operator=(const AnnoBoundingBox &r);

            public:
                virtual void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                virtual void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);

            public:
                void setImage(const QImage *segmImg);
                QImage *getImage();

            private:
                QImage *_segmImage;
        };

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
