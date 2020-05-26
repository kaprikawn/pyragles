#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class Entity;

class Component {
  
  private :
    
    friend class Entity;
    Entity* parent_;
    
  public :
    Component();
    ~Component();
    
    Entity* getParent() { return parent_; }
    virtual bool initialize();
    virtual void update( float dt ) {}
    virtual bool shutdown();
    
};
    
#endif //COMPONENT_HPP
