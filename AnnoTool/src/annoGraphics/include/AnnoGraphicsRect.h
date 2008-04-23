#ifndef ANNOGRAPHICSRECT_H_
#define ANNOGRAPHICSRECT_H_

#include <QGraphicsRectItem>
#include "AnnoGraphicsShape.h"

namespace anno {
    namespace graphics {

        class AnnoGraphicsRect : public QGraphicsRectItem, public AnnoGraphicsShape {
            private:
                void setupAppearance();

            public:
                AnnoGraphicsRect(dt::Annotation *anno, QGraphicsItem *parent = 0);
                AnnoGraphicsRect(dt::Annotation *anno, const QRectF &rect,
                                 QGraphicsItem *parent = 0);
                AnnoGraphicsRect(dt::Annotation *anno, qreal x, qreal y, qreal width,
                                 qreal height, QGraphicsItem *parent = 0);
                virtual ~AnnoGraphicsRect();

            public:
                virtual QGraphicsItem *graphicsItem();
                virtual void paint(QPainter *painter,
                                   const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
        };

    }
}

#endif /*ANNOGRAPHICSRECT_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
