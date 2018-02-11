#ifndef SHADER_HPP
#define SHADER_HPP

#include "SDL.h"
#include <GLES2/gl2.h>
#include <string>

class Shader {
  private:
    GLuint    programObject_;
    GLfloat   vertices_[18];
    GLuint    elements_[6];
    
    GLuint    vao_;
    GLuint    ebo_;
    
    
    GLint     positionLoc_;
    GLint     mvpLoc_;
    
  public:
    GLuint  load( const char* shaderSrc, GLenum type );
    int     init();
    void    update( float dt );
    void    render();
    
};

#endif //SHADER_HPP
