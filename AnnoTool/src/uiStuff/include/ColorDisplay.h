#ifndef COLORDISPLAY_H_
#define COLORDISPLAY_H_

#include <QWidget>
#include <QColor>
#include <QBrush>
#include <QPen>

class QPaintEvent;

class ColorDisplay : public QWidget {
        Q_OBJECT

    private:
        QBrush _curBrush;
        QPen _borderPen;

    public:
        ColorDisplay(QWidget *parent = 0);
        ColorDisplay(const QColor &color, QWidget *parent = 0);

    protected:
        virtual void paintEvent(QPaintEvent *event);

    public:
        void setColor(const QColor &color);
        QColor getColor() const;
};

#endif /* COLORDISPLAY_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
