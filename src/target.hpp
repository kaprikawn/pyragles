#ifndef TARGET_HPP
#define TARGET_HPP

#include <vector>
#include "gameObject.hpp"

class Target : public GameObject {
  
  private :
    
    std::vector<float> vertexData_;
    
  public :
    Target();
    virtual ~Target();
    
    virtual void update( float dt, glm::vec4 shipPosition, float shipAngleX, float shipAngleY );
    virtual void render( glm::mat4 viewProjectionMatrix );
    
};
    
#endif //TARGET_HPP
