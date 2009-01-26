#ifndef ANNOGRAPHICSVIEW_H_
#define ANNOGRAPHICSVIEW_H_

#include <QGraphicsView>

class AnnoGraphicsView : public QGraphicsView {
        Q_OBJECT

    private:
        bool _openGl;

    private:
        void setupView();
        void setupBack();

    protected:
        virtual void drawBackground(QPainter *painter, const QRectF &rect);
        virtual void wheelEvent(QWheelEvent *event);
        virtual void keyReleaseEvent(QKeyEvent *event);



    public:
        AnnoGraphicsView(QWidget *parent = 0);
        AnnoGraphicsView(QGraphicsScene *scene, QWidget *parent = 0);
        virtual ~AnnoGraphicsView();

        bool isOpenGl() const;
};


inline bool AnnoGraphicsView::isOpenGl() const {
    return _openGl;
}

#endif /* ANNOGRAPHICSVIEW_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
