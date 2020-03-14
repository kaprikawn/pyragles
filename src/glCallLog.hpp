#ifndef GLCALLLOG_HPP
#define GLCALLLOG_HPP

#include <signal.h>

#define ASSERT(x) if( !(x)) raise(SIGINT);
#define GLCall(x) GLClearError();\
  x;\
  ASSERT( GLLogCall( #x, __FILE__, __LINE__ ) )

  
void GLClearError();

bool GLLogCall( const char* function, const char* file, int line );

#endif //GLCALLLOG_HPP
