#ifndef HERO_HPP
#define HERO_HPP

#include "glObject.hpp"

class Hero : public GlObject {
  private:
  
    GLfloat   xVelocity_ = 0.0f;
    GLfloat   yVelocity_ = 0.0f;
    
  public:
    Hero();
    virtual ~Hero(){}
    
    void handleInput( float dt );
    
    void calculateRotation( float dt );
    void updatePosition( float dt );
    
    virtual void update( float dt );
    virtual void render();
    virtual void clean();
    
};

#endif //HERO_HPP
