#ifndef GLWINDOW_HPP
#define GLWINDOW_HPP

#include "SDL.h"
#include <GLES2/gl2.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define PI 3.1415926535897932384626433832795f

class GlWindow {
  private:
    GLuint                programID_;
    std::vector<GLfloat>  verts_;
    std::vector<GLushort> indices_;
    GLuint                vbo_;
    GLuint                ibo_;
    GLint                 positionID_;
    
  public:
    
    int     init();
    void    update( float dt );
    void    render();
    
};

#endif //GLWINDOW_HPP
