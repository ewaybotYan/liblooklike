#include "../include/openglwindow.hpp"

#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>

#include <GL/gl.h>

OpenGLWindow::OpenGLWindow(){
  setSurfaceType(QWindow::OpenGLSurface);
  // Specify the format and create platform-specific surface
  QSurfaceFormat format;
  format.setDepthBufferSize( 32 );
  format.setMajorVersion( 2 );
  format.setMinorVersion( 1 );
  format.setSamples( 4 );
  format.setProfile( QSurfaceFormat::CoreProfile );
  setFormat( format );
  create();

  // Create an OpenGL context
  m_context = new QOpenGLContext;
  m_context->setFormat( format );
  m_context->create();

  // Make the context current on this window
  m_context->makeCurrent( this );

//  m_funcs = m_context->versionFunctions<QOpenGLFunctions_2_1_CoreBackend>();
  if ( !m_funcs ) {
    qWarning( "Could not obtain OpenGL versions object" );
//    exit( 1 );
  }
//  m_funcs->initializeOpenGLFunctions();
}



void OpenGLWindow::render(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLWindow::showRender(){
  if(!isExposed())
    return;

  m_context->makeCurrent(this);

  m_context->swapBuffers( this );
}



bool OpenGLWindow::event(QEvent *event)
{
  switch (event->type()) {
    case QEvent::UpdateRequest:
      render();
      showRender();
      return true;
    default:
      return QWindow::event(event);
  }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
  Q_UNUSED(event);

  if (isExposed()){
    render();
    showRender();
  }
}

void OpenGLWindow::resizeEvent(QResizeEvent * event){
  Q_UNUSED(event);

  if (isExposed()){
    render();
    showRender();
  }
}
