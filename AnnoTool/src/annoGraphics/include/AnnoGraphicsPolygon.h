#ifndef ANNOGRAPHICSPOLYGON_H_
#define ANNOGRAPHICSPOLYGON_H_

#include <QGraphicsPolygonItem>
#include "AnnoGraphicsControlPoint.h"

class QGraphicsSceneMouseEvent;
using ::anno::graphics::AnnoGraphicsControlPoint;

class AnnoGraphicsPolygon : public QObject, public QGraphicsPolygonItem {
        Q_OBJECT

    private:
        QVector<AnnoGraphicsControlPoint *> _controlPoints;

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    private:
        void validateCP();

    public:
        AnnoGraphicsPolygon();
        virtual ~AnnoGraphicsPolygon();

        void initControlPoints();
        void insertControlPoint(int index, AnnoGraphicsControlPoint *cp);
        void removeControlPoint(int index);
        void moveControlPoint(int index, qreal x, qreal y);

        void setupAppearance();
};

#endif /*ANNOGRAPHICSPOLYGON_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
