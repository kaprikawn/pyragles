#ifndef SHADER_HPP
#define SHADER_HPP

#include "SDL.h"
#include <GLES2/gl2.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define PI 3.1415926535897932384626433832795f

class Shader {
  private:
    GLuint                programObject_;
    std::vector<GLfloat>  vertices_;
    std::vector<GLuint>   indices_;
    
    GLuint    vbo_;
    GLuint    ibo_;
    
    
    GLint     positionLoc_;
    GLint     mvpLoc_;
    
  public:
    GLuint  load( const char* shaderSrc, GLenum type );
    int     init();
    void    update( float dt );
    void    render();
    
};

#endif //SHADER_HPP
