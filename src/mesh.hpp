#ifndef MESH_HPP
#define MESH_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>

struct AABB {
  GLfloat   top;
  GLfloat   bottom;
  GLfloat   left;
  GLfloat   right;
  GLfloat   front;
  GLfloat   back;
};

class Mesh {
  
  private:
    
    std::vector<glm::vec3> originalVertices_;
    std::vector<glm::vec3> vertices_;
    
    std::vector<std::array<glm::vec3, 3>> mesh_;
    
    AABB aabb_;
    
    glm::vec3 position_;
    
  public:
    
    Mesh( glm::vec3 initPosition, std::vector<glm::vec3> vertices, bool print = false );
    ~Mesh(){}
    
    void updatePosition( glm::vec3 velocity, GLfloat dt, bool skip = false );
    void updateMesh( glm::mat4 modelMatrix );
    
    void setX( GLfloat x );
    void setY( GLfloat y );
    void setPosition( glm::vec3 coordinates );
    
    bool print_ = false;
    
    glm::vec3 position() {
      return position_;
    }
    
    AABB aabb() { return aabb_; }
    std::vector<glm::vec3> vertices() { return vertices_; }
    
    GLfloat x() { return position_.x; }
    GLfloat y() { return position_.y; }
    GLfloat z() { return position_.z; }
    
};

#endif // MESH