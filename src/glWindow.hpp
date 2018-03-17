#ifndef GLWINDOW_HPP
#define GLWINDOW_HPP

#include "SDL.h"
#include <GLES2/gl2.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define PI 3.1415926535897932384626433832795f

struct Vertex {
  glm::vec3   position;
  glm::vec3   colour;
};

class GlWindow {
  private:
    GLuint    programID_;
    //GLfloat   vertices_[9];
    Vertex    myTri_[1000];
    GLint     positionID_;
    GLint     colourID_;
    
  public:
    
    int     init();
    void    update( float dt );
    void    render();
    
};

#endif //GLWINDOW_HPP
