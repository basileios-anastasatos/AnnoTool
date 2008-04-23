#include "include/AnnoGraphicsRect.h"
#include "GlobalConfig.h"
#include <QMap>
#include <QGraphicsPixmapItem>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QStyle>
#include <QStyleOptionGraphicsItem>


namespace anno {
    namespace graphics {

        AnnoGraphicsRect::AnnoGraphicsRect(dt::Annotation *anno,
                                           QGraphicsItem *parent) :
            QGraphicsRectItem(parent), AnnoGraphicsShape(anno) {
            setupAppearance();
        }

        AnnoGraphicsRect::AnnoGraphicsRect(dt::Annotation *anno,
                                           const QRectF &rect, QGraphicsItem *parent) :
            QGraphicsRectItem(rect, parent), AnnoGraphicsShape(anno) {
            setupAppearance();
        }

        AnnoGraphicsRect::AnnoGraphicsRect(dt::Annotation *anno, qreal x, qreal y,
                                           qreal width, qreal height, QGraphicsItem *parent) :
            QGraphicsRectItem(x, y, width, height, parent), AnnoGraphicsShape(anno) {
            setupAppearance();
        }

        AnnoGraphicsRect::~AnnoGraphicsRect() {
        }

        QGraphicsItem *AnnoGraphicsRect::graphicsItem() {
            return this;
        }

        void AnnoGraphicsRect::paint(QPainter *painter,
                                     const QStyleOptionGraphicsItem *option, QWidget *widget) {
            QStyleOptionGraphicsItem option2(*option);
            if(isSelected()) {
                setPen(GlobalConfig::shapeColors.penSelected);
                setBrush(GlobalConfig::shapeColors.brushSelected);
                option2.state = option2.state & ~QStyle::State_Selected;
            } else {
                setPen(GlobalConfig::shapeColors.penNormal);
                setBrush(GlobalConfig::shapeColors.brushNormal);
            }
            QGraphicsRectItem::paint(painter, &option2, widget);
        }

        void AnnoGraphicsRect::setupAppearance() {
            QPen pen(QColor(30, 30, 255, 255));
            pen.setWidth(1);
            setPen(pen);
            setBrush(QBrush(QColor(255, 255, 60, 45)));
            setFlag(QGraphicsItem::ItemIsSelectable);
            setVisible(true);
        }

    }
}

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
