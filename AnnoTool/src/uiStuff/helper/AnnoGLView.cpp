#include "include/AnnoGLView.h"
#include "importGlobals.h"



QGLFormat AnnoGLView::_glFormat(QGL::Rgba | QGL::AlphaChannel | QGL::SampleBuffers);

AnnoGLView::AnnoGLView(QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f) :
    QGLWidget(_glFormat, parent, shareWidget, f) {
}

AnnoGLView::~AnnoGLView() {
}

//void AnnoGLView::initializeGL()
//{
//	QGLWidget::initializeGL();
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//	GlobalLogger::instance()->logInfo("Init OpenGL");
//
////	glEnable(GL_LINE_SMOOTH);
////	glEnable(GL_POLYGON_SMOOTH);
////	glEnable(GL_POINT_SMOOTH);
////	glEnable(GL_BLEND);
////	glEnable(GL_ALPHA_TEST);
////	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
////	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
////	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//}

//void AnnoGLView::paintGL()
//{
//	glColor3b(255, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
//	QGLWidget::paintGL();
//}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
