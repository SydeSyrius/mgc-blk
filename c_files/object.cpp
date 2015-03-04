/*  Object.cpp
 * 
 * 
 */

class object {
public:
  string name;
  object *next;
  object *previous;
  object *parent;
  object *child;
  
  object(string _name, object * _previous, object *prev);
  ~object(){};
};

object::object(string _name, object * _previous, object *_parent) {
  next     = NULL;
  previous = _previous;
  parent   = _parent;
  child    = NULL;
  name     = _name;
  
}
