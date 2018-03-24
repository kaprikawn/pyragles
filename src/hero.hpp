#ifndef HERO_HPP
#define HERO_HPP

#include "glObject.hpp"

class Hero : public GlObject {
  private:
    
    GLfloat xPos    = 0.0f;
    GLfloat yPos    = 0.0f;
    
  public:
    Hero();
    virtual ~Hero(){}
    
    void handleInput( float dt );
    
    void updatePosition( float dt );
    
    virtual void update( float dt );
    virtual void render();
    virtual void clean();
    
};

#endif //HERO_HPP
