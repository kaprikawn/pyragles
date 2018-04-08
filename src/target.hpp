#ifndef TARGET_HPP
#define TARGET_HPP

#include "glObject.hpp"

class Target : public GlObject {
  
  private:
    
    std::shared_ptr<glm::vec3> heroPosition_;
    
    GLfloat   joyAxisX_;
    GLfloat   joyAxisY_;
    
    GLfloat maxValX_;
    GLfloat maxValY_;
  
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
