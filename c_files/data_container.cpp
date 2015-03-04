/*
 *  data_container.cpp
 * 
 * 
 * 
 */

class data_container {
public:
  object *head;
  
  data_container() {head = NULL;};
  ~data_container() {head = NULL;};
  
  void add_new_object(string);
  void delete_object(string n){};
  void display_data(object *disp);
};

void data_container::display_data(object *disp) {
  
  if (head==NULL) {
    cout<<"No data loaded"<<endl;
    return;
  }
  cout<<disp->name<<endl;
  if (disp->child!=NULL) display_data(disp->child);
  if (disp->next!=NULL)  display_data(disp->next);
  return;
}

void data_container::add_new_object(string name) {
 
  object *finder=head;
  
  if (finder == NULL) {
    if (name[0]!='-' ) {
      head = new object(name, NULL, NULL);
      return;
    } else {
      cout<<"ERROR: no parent for objecttt '"<<name<<" '"<<endl;
      exit (1);
    }
  } 
  
  bool stopSearching = false;
  int in=0;
  while (!stopSearching) {
    while (finder->next!=NULL) {
      finder=finder->next;
    }
    
    if (name[in]=='-') {
      if (finder->child == NULL) {
	if (finder->name[in-1]=='-' || in==0) {
	  finder->child = new object(name, NULL, finder);
	  return;
	} else {
	  cout<<"ERROR: no parent for object '"<<name<<" '"<<endl;
	  exit (1);
	}
      } else {
	finder=finder->child;
	in+=1;
	continue;
      }
    }
    stopSearching = true;
  }
  finder->next = new object(name, finder, NULL);
  
}