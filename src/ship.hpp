#ifndef SHIP_HPP
#define SHIP_HPP

#include "gameObject.hpp"
#include "inputHandler.hpp"

class Ship : public GameObject {
  
  private :
    std::shared_ptr<InputHandler> inputHandler_;
    
  public :
    Ship( std::shared_ptr<InputHandler> inputHandler );
    ~Ship();
    bool init( std::string modelName );
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
  
};
    
#endif //SHIP_HPP
