#ifndef SHIP_HPP
#define SHIP_HPP

#include "gameObject.hpp"
#include "inputHandler.hpp"

class Ship : public GameObject {
  
  private :
    std::shared_ptr<InputHandler> inputHandler_;
    
    float zAngle_   = 0.0f;
    
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
