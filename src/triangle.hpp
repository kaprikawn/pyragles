#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "SDL.h"
#include <GLES2/gl2.h>

class Triangle {
  private:
    
    SDL_GLContext     context_;
    GLuint            u_time_loc;
    float             u_time;
    static Triangle*  instance_;
    
    Triangle(){}
    
  public:
    ~Triangle(){}
    
    void init();
    void render();
    
    static Triangle* Instance() {
      if( instance_ == 0 ) {
        instance_ = new Triangle();
      }
      return instance_;
    }
    
};

typedef Triangle TheTriangle;

#endif //TRIANGLE_HPP
