#ifndef EXHAUST_HPP
#define EXHAUST_HPP

#define PARTICLE_COUNT 20

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <random>
#include "gameObject.hpp"

struct Particle {
  glm::vec3 position      = { 0.0f, 0.0f, 0.0f };
  glm::vec4 colour        = { 1.0f, 1.0f, 0.0f, 1.0f };
  float     scale         = 0.1f;
  float     scaleIncrease = 0.1f;
  bool      active        = false;
  glm::vec3 direction     = { 0.0f, 1.0f, 0.0f };
};

class Exhaust : public GameObject {
  
  private :
  
    unsigned int        rendererID_;
    std::mt19937 eng{std::random_device{}()};
    std::uniform_int_distribution<int> zeroToHundread{ 0, 100 };
    std::uniform_int_distribution<int> zeroToOne{ 0, 100 };
    
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
    Exhaust();
    ~Exhaust();
    
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
    
    void spawnParticle( glm::vec3 newPosition, float xAngle, float yAngle );
    
    float randomFloat();
    bool  randomBool();
    
};
    
#endif //EXHAUST_HPP
