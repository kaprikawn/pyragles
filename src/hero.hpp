#ifndef HERO_HPP
#define HERO_HPP

#include "glObject.hpp"
#include "target.hpp"

class Hero : public GlObject {
  
  private:
  
    Target*   target_;
    GLfloat   maxRotationZ_ = 10.0f;
    
    GLfloat   heroX_;
    GLfloat   heroY_;
    GLfloat   targetX_;
    GLfloat   targetY_;
  
    GLfloat   buffer  =  0.3f;
    GLfloat   catchup = 90.0f;
    
  public:
    Hero( int shapeType, Target* target );
    virtual ~Hero(){}
    
    void calculateRotation( float dt );
    void handleInput      ( float dt );
    void updatePosition   ( float dt );
    
    virtual void update   ( float dt );
    virtual void render();
    virtual void clean();
  
};

#endif //HERO_HPP
