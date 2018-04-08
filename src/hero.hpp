#ifndef HERO_HPP
#define HERO_HPP

#include "glObject.hpp"
#include "target.hpp"

class Hero : public GlObject {
  
  private:
  
    std::shared_ptr<Target>     target_;
    std::shared_ptr<glm::vec3>  heroPosition_;
    
    GLfloat   maxRotationZ_ = 10.0f;
    
    GLfloat   heroX_;
    GLfloat   heroY_;
    GLfloat   targetX_;
    GLfloat   targetY_;
    
  public:
    Hero( int shapeType, GLuint programID, std::shared_ptr<Target> target, std::shared_ptr<glm::vec3> heroPosition );
    virtual ~Hero(){}
    
    virtual void calculateRotation( float dt );
    void handleInput      ( float dt );
    void updatePosition   ( float dt );
    
    virtual void update   ( float dt );
    virtual void render();
    virtual void clean();
    
};

#endif //HERO_HPP
