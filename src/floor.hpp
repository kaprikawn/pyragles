#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "gameObject.hpp"
#include "global.hpp"

class Floor : public GameObject {
  
  private :
  
    float zPos_ = FLOOR_Y;
    
  public :
    Floor();
    ~Floor();
    
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
  
};
    
#endif //FLOOR_HPP
