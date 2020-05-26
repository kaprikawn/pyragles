#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>

class Component;

class Entity {
  
  private :
  
  
    static const unsigned int NUM_MAX_COMPONENTS = 10;
    Component* components_[ NUM_MAX_COMPONENTS ];
    unsigned int numComponents_;
    
  public :
    Entity();
    ~Entity();
    
    glm::vec4 position_;
    
    bool initalize();
    void addComponent( Component* component );
    void update( float dt );
    bool shutdown();
    
};
    
#endif //ENTITY_HPP
