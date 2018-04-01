#ifndef HERO_HPP
#define HERO_HPP

#include "glObject.hpp"
#include "target.hpp"

class Hero : public GlObject {
  
  private:
  
    Target*   target_;
    //Vector    targetCoordinates_;
    
  public:
    Hero( int shapeType, Target* target );
    virtual ~Hero(){}
    
    void calculateRotation();
    
    virtual void update( float dt );
    virtual void render();
    virtual void clean();
  
};

#endif //HERO_HPP
