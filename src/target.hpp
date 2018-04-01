#ifndef TARGET_HPP
#define TARGET_HPP

#include "glObject.hpp"

class Target : public GlObject {
  
  private:
    
    GLfloat   joyAxisX_;
    GLfloat   joyAxisY_;
    
    GLfloat maxValX_;
    GLfloat maxValY_;
  
  public:
    Target( int shapeType );
    virtual ~Target(){}
    
    void handleInput( float dt );
    //void updatePosition( float dt );
    
    virtual void update( float dt );
    virtual void render();
    virtual void clean();
    
    Position position() { return position_; }
  
};

#endif //TARGET_HPP
