#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <math.h>
#include <GLES2/gl2.h>

class Vector {
  
  private:
    GLfloat x_;
    GLfloat y_;
    GLfloat z_;
  
  public:
    Vector( GLfloat x, GLfloat y, GLfloat z ) : x_( x ), y_( y ), z_( z ) {}
    ~Vector(){}
    
    GLfloat x() { return x_; }
    GLfloat y() { return y_; }
    GLfloat z() { return z_; }
    
    void setX( GLfloat x ) { x_ = x; }
    void setY( GLfloat y ) { y_ = y; }
    void setZ( GLfloat z ) { z_ = z; }
    
    float length() { return sqrt( x_ * x_ + y_ * y_ + z_ * z_ ); }
    
    Vector operator+( const Vector& v2 ) const {
      return Vector( x_ + v2.x_, y_ + v2.y_, z_ + v2.z_ );
    }
    
    friend Vector& operator+=( Vector& v1, const Vector& v2 ) {
      v1.x_ += v2.x_;
      v1.y_ += v2.y_;
      v1.z_ += v2.z_;
      return v1;
    }
    
    Vector operator*( float scaler ) {
      return Vector( x_ * scaler, y_ * scaler, z_ * scaler );
    }
    
    Vector& operator*=( float scaler ) {
      x_ *= scaler;
      y_ *= scaler;
      z_ *= scaler;
      return *this;
    }
    
    void normalize() {
      float l = length();
      if( l > 0 ) {
        ( *this ) *= 1 / l;
      }
    }
  
};

#endif //VECTOR_HPP
