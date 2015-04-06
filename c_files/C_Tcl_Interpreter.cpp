/*
 * C_Tcl_interface.cpp
 * 
 * Commands:
 *  - wczytaj_plik_wejsciowy
 *  - zapisz_plik_wynikowy
 *  - wykonaj_skrypt
 *  - raportuj_obiekt
 *  - usun_obiekt -> removeFromClist
 *  - dodaj_obiekt -> addToClist
 *  - zmien_litere
 *
 */


class C_Tcl_interface {
    public:
    Tcl_Interp *interp;
    Object *root;

    int argss_n; 
    cmdLineMessage message;
    
    C_Tcl_interface();
    static int AddObjCmd (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
      return (reinterpret_cast<C_Tcl_interface*>(clientData))->LinkCommand (interp, objc, objv);
    }
    int LinkCommand (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int cmdLineHandling (int argc, char *argv[]);
    void tcl_main(int argc, char* argv[]);
    void InitializeCommand(string) ;
    void InitializeCommands() ;
    void getHierarchyTable (string option, string hierarchy, string hierarchyTable[]);
		
    // Registered cmds
    int readInputDoFile (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int addToClist (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int removeFromClist (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
		void printStructure (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], string& outputString, const string& parent, const string& child); 
    int displayStructure(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int writeFile(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int memoryUsage(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
};


C_Tcl_interface :: C_Tcl_interface () {
	//  Tcl_FindExecutable
  interp = Tcl_CreateInterp();
  if (TCL_OK != Tcl_Init (interp)) {
    fprintf (stderr, "Tcl_Init error: %s\n", Tcl_GetStringResult (interp));
    exit (EXIT_FAILURE);
  }
  
  root = new Object();
}

void C_Tcl_interface::tcl_main(int argc, char* argv[]) {
  
  // Link Commands commands
  InitializeCommands();
  cmdLineHandling (argc, argv);
}

int C_Tcl_interface::cmdLineHandling (int argc, char *argv[]) {
  string cmd;
  if (argc == 1) {
    message.printBanner();
    cmd = "source tcl_files/tcl_init_variables.tcl\nsource Tcl_int.tcl";
  } else if (argv[1] == std::string("-help")) {
    if (argc > 2) {
      message.printError();
    } else {
      message.printHelp();
    } 
  } else if (argv[1] == std::string("-execute_script")) {
    if (argv[2] != NULL) {
      if (argc>3){
				message.printError();
      } else {
				message.printBanner();
				cmd = "source tcl_files/tcl_init_variables.tcl\nexecute_script " + std::string(argv[2]) + "\nsource Tcl_int.tcl";
      }
    } else {
      message.printError();
    }
  } else {
    message.printError();
  }
  // Run tcl console
  if (TCL_OK != Tcl_Eval (interp, cmd.c_str())) {
    cout<<"[ERROR] "<<Tcl_GetStringResult (interp)<<endl;
    exit (1);
  }
  return 0;
}

void C_Tcl_interface::InitializeCommand(string command_name) {
  Tcl_CreateObjCommand(interp,command_name.c_str(), AddObjCmd, (ClientData) this, (Tcl_CmdDeleteProc *) NULL);
}

void C_Tcl_interface::InitializeCommands() {
  // add your cammand name here to be callable from Tcl shell
  InitializeCommand("execute_script");
  InitializeCommand("add_object");
  InitializeCommand("delete_object");
  InitializeCommand("display_object");
  InitializeCommand("write_file");
  InitializeCommand("memory_usage");
}

int C_Tcl_interface::LinkCommand (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    // This is a command parser
    // objc                    -> number of arguments    
    // objv[0]                 -> command name
    // objv[1] to objv[objc-1] -> arguments
    string commandName = Tcl_GetString(objv[0]);
    
    if (commandName=="add_object") {
      return addToClist(interp, objc, objv) ;
    } else if (commandName=="delete_object") {
        return removeFromClist(interp, objc, objv) ;
    } else if (commandName=="display_object") {
      return displayStructure(interp, objc, objv) ;
    } else if (commandName=="write_file") {
      return writeFile(interp, objc, objv) ;
    } else if (commandName=="execute_script") {
       return readInputDoFile(interp, objc, objv) ;
    } else if (commandName=="memory_usage")  {
      return memoryUsage(interp, objc, objv) ;
    }

  
  return TCL_OK;
}

int C_Tcl_interface::readInputDoFile(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

  if (2 != objc) {
	  Tcl_WrongNumArgs (interp, 1, objv, "<file_name>");
	  return TCL_ERROR;
  }

  cout<<"Executing: "<<  Tcl_GetString(objv[1]) << endl ;
  if (TCL_OK != Tcl_EvalFile(interp,Tcl_GetString(objv[1]))) {
	  return TCL_ERROR;
  }
  return 0;
}
  
void  C_Tcl_interface::getHierarchyTable (string option, string hierarchy, string hierarchyTable[]) {
  if (option!="-below")  {
    return;
  }
  int startString = 0;
  int index = 0;
  size_t findIndex =hierarchy.find("/");
  
  while (findIndex != string::npos) {    
    startString = startString;

    hierarchyTable[index] = hierarchy.substr(startString,findIndex);
    hierarchy.replace(0,findIndex+1,"");
    index++;
    findIndex = hierarchy.find("/");
  }
  hierarchyTable[index] = hierarchy;
  return;
}

int C_Tcl_interface::addToClist(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
  string object;
   // add_object <string> [-below <string>]
  if ((2 != objc) & (4 != objc)) {
    Tcl_WrongNumArgs (interp, 1, objv, "<string> [ -below <string> ]");
    return TCL_ERROR;
  } 

//	string validateCmd="validateCmdLine";
  // string - object
  object = Tcl_GetString (objv[1]);

  // Add to tcl list first
  string invokeProc="add_to_single_name " + object; 
  Tcl_Eval(interp, invokeProc.c_str());
  // good place to insert bug :D
  string hierarchyTable[10]="";

  // according to LR present version
  string parent = "";
  string child  = "";
  if (objc == 4) {
    getHierarchyTable(Tcl_GetString(objv[2]), Tcl_GetString (objv[3]), hierarchyTable);
    parent = hierarchyTable[0];
    child  = hierarchyTable[1]; 
  }
  Object *tmp;
  // Adding parent (no below)
  if (parent=="") {
 	root->addChild(object,"");
  } else {
	tmp=root->findChild(parent);
	if (tmp == NULL ) {
		cout << "Wrong hierarchy" << endl;
	} else {
		if (child=="") {
			tmp->addChild(object, "-");
		} else {
			tmp = tmp->findChild(child);
			if (tmp == NULL) {
				   cout << "Wrong hierarchy" << endl;
			} else {
				tmp->addChild(object, "--");
			}
		}
	}
  }
  return TCL_OK;
}

int C_Tcl_interface::removeFromClist(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

  string object="";

	if ((1!= objc) & (2 != objc) & (4 != objc)) {
    Tcl_WrongNumArgs (interp, 1, objv, "[<string>] [ -below <string> ]");
    return TCL_ERROR;
  }
 
	// string - object
	if (objc !=1) {
	  object = Tcl_GetString (objv[1]);
	}
	// good place to insert bug :D
  string hierarchyTable[10]="";

  string parent = "";
  string child  = "";
  if (objc == 4) {
    getHierarchyTable(Tcl_GetString(objv[2]), Tcl_GetString (objv[3]), hierarchyTable);
    parent = hierarchyTable[0];
    child  = hierarchyTable[1];
  } 
 
	string toRemove;
  Object *tmp;
  // Adding parent (no below)
  if (parent=="") {
		root->removeChild(toRemove);
  } else {
		tmp=root->findChild(parent);
		if (tmp == NULL ) {
			cout << "Wrong hierarchy" << endl;
		} else {
			if (child=="") {
				tmp->removeChild(toRemove);
			} else {
				tmp = tmp->findChild(child);
				if (tmp == NULL) {
					cout << "Wrong hierarchy" << endl;
				} else {
					tmp->removeChild(toRemove);
				}
			}
		}
	}
  string invokeProc="removeFromSingle {" + toRemove + "}";
  Tcl_Eval(interp, invokeProc.c_str());
  
	return TCL_OK;
}

int C_Tcl_interface::displayStructure (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

  if (1!=objc & 3 != objc) {
    Tcl_WrongNumArgs (interp, 1, objv, "object name");
    return TCL_ERROR;
  }

	// good place to insert bug :D
  string hierarchyTable[10]="";
  string parent = "";
  string child  = "";
  if (objc == 3) {
    getHierarchyTable(Tcl_GetString(objv[1]), Tcl_GetString (objv[2]), hierarchyTable);
    parent = hierarchyTable[0];
    child  = hierarchyTable[1];
  }

	string outputString="";
	printStructure(interp, objc, objv, outputString,parent,child);
	cout<<outputString;
  return TCL_OK; 
}

void C_Tcl_interface::printStructure (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], string& outputString, const string& parent, const string& child){
	Object* tmp;
	if (parent=="") {
		root->printName(outputString);
	} else {
		tmp=root->findChild(parent);
		if (tmp == NULL ) {
			cout << "Wrong hierarchy" << endl;
		} else {
			if (child=="") {
				tmp->printName(outputString);
			} else {
				tmp = tmp->findChild(child);
				if (tmp == NULL) {
					cout << "Wrong hierarchy" << endl;
				} else {
					tmp->printName(outputString);
					}
				}
			}
		}
}

int C_Tcl_interface::writeFile (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
   
  if (root->child==NULL) { // Ugly 
		cout << "[ERROR] Nothing to save" << endl;
		return 1;
  }
  if (2 != objc) {
          Tcl_WrongNumArgs (interp, 1, objv, "<file_name>");
          return TCL_ERROR;
  }
	
	if (Tcl_Eval(interp, Tcl_GetString(objv[1]))) {
		string outputString="";
  	ofstream fh(Tcl_GetString(objv[1]));
    printStructure(interp, objc, objv, outputString, "", "");
		fh<<outputString;
		fh.close();
	} else {
		cout << "[ERROR} File already exists" << endl;
	}
}

int C_Tcl_interface::memoryUsage (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

   if (1 < objc) {
     // do poprawy
    //Tcl_WrongArgs (interp, 1, objv, "Ta opcja nie przyjmuje zadnych argumentow.");
    return TCL_ERROR;
  }
  cout << "[Info] Memory usage is " << getValue() << endl;
  return TCL_OK;
}
