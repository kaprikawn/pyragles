#ifndef HERO_HPP
#define HERO_HPP

#include "glObject.hpp"

class Hero : public GlObject {
  private:
  
    
  public:
    Hero();
    virtual ~Hero(){}
    
    virtual void update( float dt );
    virtual void render();
    virtual void clean();
    
    void handleInput();
    
  
};

#endif //HERO_HPP
