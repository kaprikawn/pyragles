#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "gameObject.hpp"

struct Vertex {
  float position[3];
  float colour[4];
};

class Particles : public GameObject {
  
  private :
  
    unsigned int        rendererID_;
    
    std::vector<float>  myPoint_;
    
  public :
    Particles();
    ~Particles();
    
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
    
};
    
#endif //PARTICLES_HPP
