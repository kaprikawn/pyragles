#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#define PARTICLE_COUNT 1

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "gameObject.hpp"

struct Particle {
  glm::vec3 position  = { 0.0f, 0.0f, 0.0f };
  float     scale     = 0.1f;
  bool      active    = true;
};

class Particles : public GameObject {
  
  private :
  
    unsigned int        rendererID_;
    Particle            particles_[ PARTICLE_COUNT ];
    
    float cubeVertices_[ 108 ] = {
        -1.0f, -1.0f, -1.0f
      , -1.0f, -1.0f,  1.0f
      , -1.0f,  1.0f,  1.0f
      ,  1.0f,  1.0f, -1.0f
      , -1.0f, -1.0f, -1.0f
      , -1.0f,  1.0f, -1.0f
      ,  1.0f, -1.0f,  1.0f
      , -1.0f, -1.0f, -1.0f
      ,  1.0f, -1.0f, -1.0f
      ,  1.0f,  1.0f, -1.0f
      ,  1.0f, -1.0f, -1.0f
      , -1.0f, -1.0f, -1.0f
      , -1.0f, -1.0f, -1.0f
      , -1.0f,  1.0f,  1.0f
      , -1.0f,  1.0f, -1.0f
      ,  1.0f, -1.0f,  1.0f
      , -1.0f, -1.0f,  1.0f
      , -1.0f, -1.0f, -1.0f
      , -1.0f,  1.0f,  1.0f
      , -1.0f, -1.0f,  1.0f
      ,  1.0f, -1.0f,  1.0f
      ,  1.0f,  1.0f,  1.0f
      ,  1.0f, -1.0f, -1.0f
      ,  1.0f,  1.0f, -1.0f
      ,  1.0f, -1.0f, -1.0f
      ,  1.0f,  1.0f,  1.0f
      ,  1.0f, -1.0f,  1.0f
      ,  1.0f,  1.0f,  1.0f
      ,  1.0f,  1.0f, -1.0f
      , -1.0f,  1.0f, -1.0f
      ,  1.0f,  1.0f,  1.0f
      , -1.0f,  1.0f, -1.0f
      , -1.0f,  1.0f,  1.0f
      ,  1.0f,  1.0f,  1.0f
      , -1.0f,  1.0f,  1.0f
      ,  1.0f, -1.0f,  1.0f
    };
    
  public :
    Particles();
    ~Particles();
    
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
    
};
    
#endif //PARTICLES_HPP
