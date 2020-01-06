#ifndef INDEXBUFFER_HPP
#define INDEXBUFFER_HPP

class IndexBuffer {

  private :
    unsigned int rendererID_;
    unsigned int count_;

  public :
    IndexBuffer();
    ~IndexBuffer();

    void init( const void* data, unsigned int count );
    void bind() const;
    void unbind() const;

    inline unsigned int getCount() const { return count_; }

};

#endif //INDEXBUFFER_HPP
