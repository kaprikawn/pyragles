#ifndef SHADER_HPP
#define SHADER_HPP

#include <GLES2/gl2.h>

class Shader {
  private:
    
    GLuint      programID_;
    
    GLint       positionID_;
    GLint       colourID_;
    GLint       mvpID_;
    
  public:
    GLuint init();
    
};

#endif //SHADER_HPP