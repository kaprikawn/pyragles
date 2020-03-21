#ifndef HUD_HPP
#define HUD_HPP

#include "gameObject.hpp"

class Hud : public GameObject {
  
  private :
  
    float windowWidth_;
    float windowHeight_;
    
  public :
    Hud();
    ~Hud();
    
    virtual void update();
    virtual void render();
    
};
    
#endif //HUD_HPP
