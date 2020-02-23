#ifndef SHIP_HPP
#define SHIP_HPP

#include "gameObject.hpp"

class Ship : public GameObject {
  
  private :
    
  public :
    Ship();
    ~Ship();
    
    virtual bool init( std::string modelName );
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
  
};
    
#endif //SHIP_HPP
