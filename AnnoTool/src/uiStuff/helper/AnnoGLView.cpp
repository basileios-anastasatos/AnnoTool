#include "include/AnnoGLView.h"
#include "importGlobals.h"



QGLFormat AnnoGLView::_glFormat(QGL::Rgba | QGL::AlphaChannel | QGL::SampleBuffers);

AnnoGLView::AnnoGLView(QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f) :
    QGLWidget(_glFormat, parent, shareWidget, f) {
}

AnnoGLView::~AnnoGLView() {
}



// vim:ts=4:sts=4:sw=4:tw=80:expandtab
