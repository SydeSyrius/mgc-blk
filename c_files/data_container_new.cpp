


class data_container_new {
public:
  object *head;
  
  data_container_new() ;
  ~data_container_new() {head = NULL;};
  
  void add_new_object(string);
  void add_new_object(string, string, string);
  void delete_object(string);
  void display_data(object *disp);
  void display_data() {display_data(head);};
  void getLastObjectOnLevel(object *& finder, int level);
//  void display_data();
  
};


void data_container_new::display_data(object *disp) {

  if (disp==NULL) return;
  cout<<"Name: "<<disp->name<<endl;
  display_data(disp->child); 
  display_data(disp->next); 
  
}

data_container_new::data_container_new() {
  head = new object();
  
}

void data_container_new::add_new_object(string name) {
// add on the end
  head->addChild(name);
  
}

void data_container_new::add_new_object(string name, string below1, string below2) {
  object* finder;
  // if below1 and below2 are empty strings, then add at the end of the list
  if (below1=="" && below2=="") {
    
    if (name[0]!='-') {
      head->addChild(name);
      return;
    }

    if (name[1]!='-') {
      getLastObjectOnLevel(finder,0);
    } else {
      getLastObjectOnLevel(finder,1);
    }

    if (finder==NULL) {
      cout<<"ERROR: no parent for object\'"<<name<<"\'"<<endl;
      exit (1);
    }

    finder->addChild(name);
    return
  }
  
  //  else find object    
}

void data_container_new::getLastObjectOnLevel(object *& finder, int level) {
  if (head->child==NULL) {
    finder = NULL;
    return;
  }
  
  finder=head->child;
  while(finder->next!=NULL) {
    finder=finder->next;
  }

  if (level==0) {
    return;
  } 
  
  if (level>=1) {
    if (finder->child==NULL) finder == NULL; 
   
    finder=finder->child;
    while(finder->next!=NULL) {
      finder=finder->next;
    }
  }
  
  if (level>=2) {
    if (finder->child==NULL) finder == NULL;
    finder=finder->child;
    while(finder->next!=NULL) {
      finder=finder->next;
    }
  } 
  
  return;
}
























