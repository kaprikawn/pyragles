#ifndef TARGET_HPP
#define TARGET_HPP

#include "glObject.hpp"

class Target : public GlObject {
  
  private:
    
    std::shared_ptr<glm::vec3> heroPosition_;
    
    GLfloat   joyAxisX_;
    GLfloat   joyAxisY_;
    
    GLfloat speedFactor_      = 30.0f;
  
    GLfloat maxDistFromShipX_ = 10.0f;
    GLfloat maxDistFromShipY_ = 5.0f;
    GLfloat currentX_         = 0.0f;
    GLfloat shipX_;
    GLfloat destinationX_;
    GLfloat currentY_         = 0.0f;
    GLfloat shipY_;
    GLfloat destinationY_;
    GLfloat targetX_;
    GLfloat targetY_;
  
  public:
    Target( int shapeType, GLuint programID, std::shared_ptr<glm::vec3> heroPosition );
    virtual ~Target(){}
    
    void handleInput( float dt );
    
    virtual void update( float dt );
    virtual void render();
    virtual void clean();
    
    Position position() { return position_; }
  
};

#endif //TARGET_HPP
