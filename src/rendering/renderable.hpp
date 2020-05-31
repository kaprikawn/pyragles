#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <glm/glm.hpp>

class Geometry;

class Renderable {
  
  friend class Renderer;
  
  private :
  
    Geometry* what_; // what we want to render
  
  public :
    
    glm::mat4 where_ = glm::mat4( 1.0f ); // where we're going to render it
    
};
    
#endif //RENDERABLE_HPP
