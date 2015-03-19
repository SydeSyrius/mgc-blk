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
  void delete_object(string);
  void display_data(object *disp);
  void display_data() {display_data(head);};
  string display_data_new();
  
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

string data_container::display_data_new() {
  string concatenate; 
  if (head==NULL) {
    cout<<"No data loaded"<<endl;
    return "";
  }
  object* disp = head;
  object* disp_top = head;
  object* disp_top2 = NULL;
  bool takeChild = 0;
  while (disp_top!=NULL) {
    concatenate+=disp_top->name+"\n";
    if (disp_top->child!=NULL) {
      disp_top2=disp_top->child;
      while (disp_top2!=NULL) {
	concatenate+=disp_top2->name+"\n";
	if (disp_top2->child!=NULL) {
	  disp=disp_top2->child;
	  while (disp!=NULL) {
	    concatenate+=disp->name+"\n";
	    disp=disp->next;
	  }
	}
	disp_top2=disp_top2->next;
      }
    }
    disp_top=disp_top->next; 
  }
  return concatenate;
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

    while (finder->next!=NULL) {
      finder=finder->next;
    }

    // first level
   if (name[0]=='-') {
      if (finder->child==NULL) {
	if (name[1] == '-') {
	  cout<<"ERROR: brak rodzica dla obiektu '"<<name<<" '"<<endl;
	  exit (1);
	}
	// add child
	finder->child =  new object(name, NULL, finder);
	return;
      } else {
	// go one level bottom
	finder = finder->child;
	// go to the right
	while (finder->next!=NULL) {
	  finder = finder->next;
	}
	
	if (name[1]=='-') {
	  // check second level
	  if (finder->child==NULL) {
	    finder->child = new object(name, NULL, finder);
	    return;
	  } else {
	    finder = finder->child;
	    // go to the right
	    while (finder->next!=NULL) {
	      finder = finder->next;
	    }
	    finder->next = new object(name, finder, NULL);
	    return;
	  }
	} else {
	  // add to the end
	  finder->next = new object(name, finder, NULL);
	  return;
	}
	
      }
    }
    
  finder->next = new object(name, finder, NULL);
  
}

void data_container::delete_object(string name) {
		// We know that this object is there
	  if (head==NULL) {
	    cout<<"No data loaded"<<endl;
	    return ;
	  }
	  object* disp = head;
	  object* disp_top = head;
	  object* disp_top2 = NULL;
	  bool takeChild = 0;
	  while (disp_top!=NULL) {
	  if (disp_top->name!=name) { 
	    if (disp_top->child!=NULL) {
	      disp_top2=disp_top->child;
	      while (disp_top2!=NULL) {
		if (disp_top2->name!=name) {
		if (disp_top2->child!=NULL) {
		  disp=disp_top2->child;
		  while (disp!=NULL) {
			if (disp->name!=name){
			    disp=disp->next;
			}
			else {
			// Del child child 
			    if (disp->next!=NULL) {
				if (disp->previous!=NULL){ // nie pierwszy
					disp->previous->next=disp->next;
					disp->next->previous=disp->previous;
					delete disp;
				}
				else {
					disp->next->previous=NULL; //disp->previous;// NULL bez tego sigsegv
					disp->next->parent=disp->parent; // my father is
					disp->parent->child=disp->next;	
					delete disp;
				}
			    }
			    else { // bylem jedynakiem
				 if (disp->previous==NULL) {
					disp->parent->child=NULL; // bez tego SIGSEGV 
				 }
				 else {
					disp->previous->next=NULL;
				 }
				delete disp;
			    }
			    return;
			}
		  }
		}
		//disp_top2=disp_top2->next;
		}
		else  {
			if (disp_top2->next!=NULL) {
				if (disp_top2->previous!=NULL){ // nie pierwszy
					disp_top2->previous->next=disp_top2->next;
                                        disp_top2->next->previous=disp_top2->previous;	
					// Del alll childs
				//	disp=disp_top2;
				//	while (disp->next!=NULL) {
			//			disp=disp->next;
			//			delete disp->previous;
			//		}
			//		delete disp;
	  				delete disp_top2;
				}
				else  {
					disp_top2->next->previous=NULL;
					disp_top2->next->parent=disp_top2->parent; // my father is
                                        disp_top2->parent->child=disp_top2->next;
				       // Del alll childs
                          //              disp=disp_top2;
                            //            while (disp->next!=NULL) {
                              //                  disp=disp->next;
                                //                delete disp->previous;
                                  //      }
                                    //    delete disp;
					delete disp_top2;
				}	
			}
			else { // jedynakiem
				if (disp_top2->previous==NULL) {
					disp_top2->parent->child=NULL;
				}
				else {
					disp_top2->previous->next=NULL;
				}
				// Del alll childs
                             //     disp=disp_top2;
                              //          while (disp->next!=NULL) {
                               //                 disp=disp->next;
                                //                delete disp->previous;
                                 //       }
                               // delete disp;
				delete disp_top2;
			}
			return ;
		}
        	disp_top2=disp_top2->next;
	}
      }
    }
    else {
    // del parent
	if (disp_top->next!=NULL) {
                                if (disp_top->previous!=NULL){ // nie pierwszy
                                        disp_top->previous->next=disp_top->next;
                                        disp_top->next->previous=disp_top->previous;
                                        // Del alll childs
                                //      disp=disp_top2;
                                //      while (disp->next!=NULL) {
                        //                      disp=disp->next;
                        //                      delete disp->previous;
                        //              }
                        //              delete disp;
                                        delete disp_top;
                                }
                                else  {
                                        disp_top->next->previous=NULL;
					head=disp_top->next;
                                       // Del alll childs
                          //              disp=disp_top2;
                            //            while (disp->next!=NULL) {
                              //                  disp=disp->next;
                                //                delete disp->previous;
                                  //      }
                                    //    delete disp;
                                        delete disp_top;
                                }
                        }
                        else { // jedynakiem
                                if (disp_top->previous!=NULL) {
                                        disp_top->previous->next=NULL;
                                }
                                // Del alll childs
                             //     disp=disp_top2;
                              //          while (disp->next!=NULL) {
                               //                 disp=disp->next;
                                //                delete disp->previous;
                                 //       }
                               // delete disp;
                                delete disp_top;
                        }
                        return ;

    }
    disp_top=disp_top->next;
    }
  }
