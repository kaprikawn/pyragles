#ifndef SHADER_HPP
#define SHADER_HPP

#include "SDL.h"
#include <GLES2/gl2.h>
#include <string>

class Shader {
  private:
    GLuint    programObject_;
    GLfloat   vertices_[9];
    GLuint    elements_[3];
    
    GLuint    vao_;
    GLuint    ebo_;
    
    
    GLint     positionLoc_;
    GLint     mvpLoc_;
    
  public:
    GLuint  load( const char* shaderSrc, GLenum type );
    int     init();
    void    render();
    
};

#endif //SHADER_HPP
