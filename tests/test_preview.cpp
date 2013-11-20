#include <QtGui/QGuiApplication>

#include <GL/gl.h>

#include "../include/previewwindow.hpp"

const unsigned int w = 300;
const unsigned int h = 500;

uint8_t* generateDummyImage( const unsigned int w,
    const unsigned int h){
  uint8_t* data = new uint8_t[4*w*h];
  for(int i=0; i<h; i++){
    for(int j=0; j<w; j++){
      for(int c=0;c<4;c++){
        unsigned long pxlchnl = i*4*w+4*j+c;
        if(c == 0){
          data[pxlchnl] = i;
        }else if( c == 1){
          data[pxlchnl] = j;
        }else{
          data[pxlchnl] = 0;
        }
      }
    }
  }
  return data;
}


int main(int argc, char** argv){
  QGuiApplication app(argc, argv);


  // create an empty window
  PreviewWindow window;

  // create the texture with the image
  uint8_t* image = generateDummyImage( w, h );
  glEnable( GL_TEXTURE_2D );
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1); 
  GLuint glTextureId;
  glGenTextures( 1, &glTextureId ); 
  glBindTexture( GL_TEXTURE_2D, glTextureId );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 
      w, h, 
      0, GL_RGBA, GL_UNSIGNED_BYTE, 
      image );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glBindTexture( GL_TEXTURE_2D, 0 );

  // set the window to use this texture
  window.s_glTextureId(glTextureId);
  window.s_imageSize(w,h);

  window.show();

  return app.exec();
}
