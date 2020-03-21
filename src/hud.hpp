#ifndef HUD_HPP
#define HUD_HPP

#include "gameObject.hpp"
#include <vector>

class Hud : public GameObject {
  
  private :
  
    float windowWidth_;
    float windowHeight_;
    
    std::vector<float> vertexData_;
    
    glm::mat4 proj_;
    
  public :
    Hud();
    ~Hud();
    
    virtual void update();
    virtual void render();
    
    void decreaseBombCount();
    
};
    
#endif //HUD_HPP
