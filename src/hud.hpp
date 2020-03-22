#ifndef HUD_HPP
#define HUD_HPP

#include "gameObject.hpp"
#include <vector>
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "texture.hpp"
#include "shader.hpp"

class Hud {
  
  private :
  
    float windowWidth_;
    float windowHeight_;
    
    int   prevBombCount_  = 0;
    
    VertexBuffer  healthVb_;
    VertexBuffer  bombsVb_;
    Shader        healthShader_;
    Shader        bombsShader_;
    IndexBuffer   ib_;
    Texture       texture_;
    
    std::vector<float> bombsVertexData_;
    std::vector<float> healthVertexData_;
    
    int  bombsPositionID_;
    int  bombsTexCoordID_;
    int  bombsMvpID_;
    
    int  healthPositionID_;
    int  healthColourID_;
    int  healthMvpID_;
    
    glm::mat4 proj_;
    
    unsigned int  indexCount_ = 0;
    
  public :
    Hud();
    ~Hud();
    
    virtual void update( unsigned short int bombCount );
    virtual void render();
    
    void updateBombCount( int bombCount );
    
};
    
#endif //HUD_HPP
