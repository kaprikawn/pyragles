#ifndef SHIP_HPP
#define SHIP_HPP

#include "gameObject.hpp"
#include "inputHandler.hpp"
#include "particles.hpp"

class Ship : public GameObject {
  
  private :
    std::shared_ptr<InputHandler> inputHandler_;
    std::unique_ptr<Particles>    particles_;
    
    float joyAxisX_ = 0.0f;
    float joyAxisY_ = 0.0f;
    
    float xAngle_ = 0.0f;
    float yAngle_ = 0.0f;
    float zAngle_ = 0.0f;
    
    float particleTimer_ = 0.0f;
    
  public :
    Ship( std::shared_ptr<InputHandler> inputHandler );
    ~Ship();
    bool init( std::string modelName );
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
    
    void  handleInput( float dt );
    void  calculateRotation( float dt );
  
};
    
#endif //SHIP_HPP
