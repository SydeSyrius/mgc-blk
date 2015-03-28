/*  Object.cpp
 * 
 * 
 */

class object {
public:
  string name;
  int level;
  object *next;
  object *previous;
  object *parent;
  object *child;
  
  object(string _name, object * _previous, object *prev);
  object();
  ~object(){};
  void addChild(string);
  void getObject(string);
  void findObject();
};

object::object() {
  
  next     = NULL;
  previous = NULL;
  parent   = NULL;
  child    = NULL;
  name     = "";
  
}

object::object(string _name, object * _previous, object *_parent) {
  next     = NULL;
  previous = _previous;
  parent   = _parent;
  child    = NULL;
  name     = _name;
  
}




void object::addChild(string new_object_name) {
  if (this->child == NULL) {
    cout<<"first_child"<<endl;
    child = new object(new_object_name, NULL, this);
    return;
  }

  object* finder = child;
  while(finder->next!=NULL) {
    finder=finder->next;
  }
  finder->next=new object(new_object_name, finder, this);
  return;
}


void object::getObject(string objectName) {
  
  
}





















