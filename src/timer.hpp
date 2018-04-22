#ifndef TIMER_HPP
#define TIMER_HPP

#include <GLES2/gl2.h>
#include <iostream>

class Timer {
  
  private:
  
    GLfloat timeLeft_ = 0.0f;
  
  public:
    Timer(){}
    ~Timer(){}
    
  void setCoundownTimer( GLfloat t ) {
    timeLeft_ = t;
  }
  
  void update( float dt ) {
    timeLeft_ -= dt;
    if( timeLeft_ < 0.0f )
      timeLeft_ = 0.0f;
  }
  
  GLfloat timeLeft() {
    return timeLeft_;
  }
    
  
};

#endif //TIMER_HPP