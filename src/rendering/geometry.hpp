#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <glm/glm.hpp>

class Geometry {
  
  public :
    glm::vec4*      vertices_;
    unsigned int    numVerts_;
    unsigned short* indices_;
    unsigned int    numIndices_;
  
};
    
#endif //GEOMETRY_HPP
