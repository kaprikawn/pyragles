#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

class GlObject {
  protected:
    
    
  public:
    GlObject();
    virtual ~GlObject(){}
    
    virtual void update ( float dt );
    virtual void render ();
    virtual void clean  ();
    
};

#endif //GLOBJECT_HPP
