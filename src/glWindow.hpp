#ifndef GLWINDOW_HPP
#define GLWINDOW_HPP

#include "SDL.h"
#include <GLES2/gl2.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include "structs.hpp"
#include "shape.hpp"

#define PI 3.1415926535897932384626433832795f

class GlWindow {
  private:
    
    GLuint    programID_;
    
    GLuint    vbo_;
    GLuint    ibo_;
    
    GLint     positionID_;
    GLint     colourID_;
    GLint     mvpID_;
    
    glm::mat4   projection_;
    glm::mat4   view_;
    glm::mat4   model_;
    glm::mat4   rotation_;
    glm::mat4   mvp_;
    
    GLfloat     yAngle_ = 0.0f;
    GLfloat     xAngle_ = 0.0f;
    
    std::unique_ptr<Shape>  shape_;
    
  public:
    
    int     init();
    void    update( float dt );
    void    render();
    
};

#endif //GLWINDOW_HPP
