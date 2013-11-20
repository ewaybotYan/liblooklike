#include <iostream>
#include <GL/gl.h>

#include <QtCore/QtGlobal>

#include "../include/previewwindow.hpp"

void PreviewWindow::s_glTextureId( const unsigned int id ){
    m_glTextureId = (GLuint)id;
}

void PreviewWindow::s_imageSize( const unsigned int w, const unsigned int h ){
  m_imgW = w;
  m_imgH = h;
}

void PreviewWindow::render(){
  std::cout << "render" << std::endl;
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();


  //  glEnable(GL_DEPTH_TEST);
  glBindTexture( GL_TEXTURE_2D, m_glTextureId );

//  glColor4f(1.0, 1.0, 1.0, 1.0);
  glBegin( GL_QUADS );
  glTexCoord2f( 0.f, 0.f ); glVertex2i( 0, 0 );
  glTexCoord2f( 1.f, 0.f ); glVertex2i( m_imgW, 0 );
  glTexCoord2f( 1.f, 1.f ); glVertex2i( m_imgW, m_imgH );
  glTexCoord2f( 0.f, 1.f ); glVertex2i( 0, m_imgH );
  glEnd();

  glBindTexture( GL_TEXTURE_2D, 0 ); 
}

void PreviewWindow::resizeEvent(QResizeEvent* event){
  Q_UNUSED(event);

  // Make the context current on this window
  m_context->makeCurrent( this );
  glViewport(0, 0, width(), height());

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,width(),0,height(),-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
