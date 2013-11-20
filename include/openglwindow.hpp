#ifndef OPENGLWINDOW_HPP
#define OPENGLWINDOW_HPP

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

class OpenGLWindow : public QWindow{
  public:
    OpenGLWindow();

    virtual void render();
//    virtual void initialize();

  public slots:
    void showRender();

  protected:
    bool event(QEvent* event) override;
    void exposeEvent(QExposeEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event);

    QOpenGLContext* m_context = 0;
    // the openGL gl_* functions corresponding to the context
    QOpenGLFunctions_2_1_CoreBackend* m_funcs = 0;
};

#endif
