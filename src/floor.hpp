#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include <glm/glm.hpp>

class Floor {
  
  private :
  
    VertexBuffer  vb_;
    IndexBuffer   ib_;
    Shader        shader_;
    
    int  positionID_;
    int  mvpID_;
    
    glm::mat4     modelMatrix_;
    glm::mat4     mvp_;
    
  public :
    Floor();
    ~Floor();
    
    void loadVertexData();
    void loadIndexData();
    void loadShader( const std::string& fileaname );
    
    
};
    
#endif //FLOOR_HPP
