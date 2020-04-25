#ifndef SHIP_HPP
#define SHIP_HPP

#include "gameObject.hpp"
#include "exhaust.hpp"
#include "target.hpp"

class Ship : public GameObject {
  
  private :
    std::unique_ptr<Target>   target_;
    std::unique_ptr<Exhaust>  particles_;
    
    float joyAxisX_ = 0.0f;
    float joyAxisY_ = 0.0f;
    
    float particleTimer_ = 0.0f;
    
  public :
    Ship();
    ~Ship();
    bool init( std::string modelName );
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
    
    void  handleInput( float dt );
    void  calculateRotation( float dt );
    
    glm::vec4 targetPosition() { return target_ -> position(); }
  
};
    
#endif //SHIP_HPP
