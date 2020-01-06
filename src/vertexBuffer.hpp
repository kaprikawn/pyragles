#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

class VertexBuffer {
  
  private :
    unsigned int rendererID_;
    
  public :
    VertexBuffer();
    ~VertexBuffer();
    
    void init( const void* data, unsigned int size );
    void bind() const;
    void unbind() const;
  
};
    
#endif //VERTEXBUFFER_HPP