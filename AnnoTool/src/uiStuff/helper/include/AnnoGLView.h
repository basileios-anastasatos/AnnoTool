#ifndef ANNOGLVIEW_H_
#define ANNOGLVIEW_H_

#include <QGLWidget>
#include <QGLFormat>

class AnnoGLView : public QGLWidget {
        Q_OBJECT

    public:
        static QGLFormat _glFormat;

    protected:
//		virtual void initializeGL();
//		virtual void paintGL();

    public:
        AnnoGLView(QWidget *parent = 0, const QGLWidget *shareWidget = 0, Qt::WindowFlags f = 0);
        virtual ~AnnoGLView();
};

#endif /* ANNOGLVIEW_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
