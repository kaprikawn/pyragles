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
    std::vector<GLfloat>  colours_;
    
    GLuint      vboIDs_[2];
    
    GLint       positionID_;
    GLint       colourID_;
    GLint       mvpID_;
    
    glm::mat4   projection_;
    glm::mat4   view_;
    glm::mat4   model_;
    glm::mat4   mvp_;
    
  public:
    GLuint  load( const char* shaderSrc, GLenum type );
    int     init();
    void    update( float dt );
    void    render();
    
};

#endif //SHADER_HPP
