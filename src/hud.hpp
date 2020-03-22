#ifndef HUD_HPP
#define HUD_HPP

#include "gameObject.hpp"
#include <vector>

class Hud : public GameObject {
  
  private :
  
    float windowWidth_;
    float windowHeight_;
    
    int   prevBombCount_  = 0;
    
    std::vector<float> vertexData_;
    
    glm::mat4 proj_;
    
  public :
    Hud();
    ~Hud();
    
    virtual void update( unsigned short int bombCount );
    virtual void render();
    
    void updateBombCount( int bombCount );
    
};
    
#endif //HUD_HPP
