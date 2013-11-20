#ifndef PREVIEWWINDOW_HPP
#define PREVIEWWINDOW_HPP

#include "openglwindow.hpp"


class PreviewWindow : public OpenGLWindow{
  public:
//    PreviewWindow();

    void render() override;

    void s_glTextureId( const unsigned int id );
    void s_imageSize( const unsigned int w , const unsigned int h );

  private:
    // id of the texture containing the image pixels
    GLuint m_glTextureId;
    // actual size of the image
    unsigned int m_imgW;
    unsigned int m_imgH;

  protected:
    void resizeEvent(QResizeEvent * event) override;
    
};

#endif
