#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <glm/glm.hpp>

class Geometry {
  
  friend class Renderer;
  
  public :
    glm::vec4*      vertices_;
    unsigned int    numVerts_;
    unsigned short* indices_;
    unsigned int    numIndices_;
    unsigned int    rendermode_;
  
};
    
#endif //GEOMETRY_HPP
