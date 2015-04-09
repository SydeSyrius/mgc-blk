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
    string TCL_return;

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
    int printStructure (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], string& outputString, const string& parent, const string& child); 
    int displayStructure(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int writeFile(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int memoryUsage(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    string run_Tcl_Eval(Tcl_Interp *interp, string command);
};


C_Tcl_interface :: C_Tcl_interface () {
	//  Tcl_FindExecutable
  interp = Tcl_CreateInterp();
  if (TCL_OK != Tcl_Init (interp)) {
    fprintf (stderr, "Tcl_Init error: %s\n", Tcl_GetStringResult (interp));
    exit (EXIT_FAILURE);
  }
  TCL_return="";
  root = new Object();
}

string C_Tcl_interface::run_Tcl_Eval(Tcl_Interp *interp, string command) {
  Tcl_Eval(interp,command.c_str());
  TCL_return = Tcl_GetStringResult(interp);
  return TCL_return;
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
    cout<<"[ERROR] Unexpected error from interpreter. Intercepted error "<<Tcl_GetStringResult (interp) << "." <<endl;
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
	  cout << "[ERROR] Syntax error." << endl;
	  return TCL_ERROR;
  }
	string cmdArg=Tcl_GetString(objv[1]);
	string invokeArg="sourceFile " + cmdArg;
	if("TCL_OK" != run_Tcl_Eval(interp, invokeArg.c_str())) {
      return 1;
  }
	
  return 0;
}
  
void  C_Tcl_interface::getHierarchyTable (string option, string hierarchy, string hierarchyTable[]) {
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
	string cmdArgs;

   // add_object <string> [-below <string>]
  if ((2 != objc) & (4 != objc)) {
    cout << "[ERROR] Syntax error." << endl;
    return TCL_ERROR;
  } else {
		cmdArgs=Tcl_GetString (objv[1]);
		if (4 == objc) {
			cmdArgs=cmdArgs + " " + Tcl_GetString(objv[2]) + " " + Tcl_GetString(objv[3]);
		}
		string validateCmd="validateCmdLine_addObject {" + cmdArgs + "}";
		if("TCL_OK" != run_Tcl_Eval(interp, validateCmd.c_str())) {
			return 1;
		}
	}
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
			cout << "[ERROR] Wrong hierarchy." << endl;
			return 1;
		} else {
			if (child=="") {
				tmp->addChild(object, "-");
			} else {
				tmp = tmp->findChild(child);
				if (tmp == NULL) {
				   cout << "[ERROR] Wrong hierarchy." << endl;
					 return 1;
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
	string cmdArgs;
	if ((1!= objc) & (2 != objc) & (4 != objc)) {
    cout << "[ERROR] Syntax error." << endl;
    return TCL_ERROR;
  } else {
    if (objc > 1) {
			cmdArgs=Tcl_GetString (objv[1]);
			object = Tcl_GetString (objv[1]);
		}
    if (4 == objc) {
      cmdArgs=cmdArgs + " " + Tcl_GetString(objv[2]) + " " + Tcl_GetString(objv[3]);
    }
    string validateCmd="validateCmdLine_deleteObject {" + cmdArgs + "}";
		if("TCL_OK" != run_Tcl_Eval(interp, validateCmd.c_str())) {
      return 1;
    }
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
 
	string toRemove="";
  Object *tmp;
  // Remove parent (no below)
  if (object=="") {
		root->removeChild(toRemove); // Remove ALL
  } else {
		if (parent=="") {
			tmp=root->findChild(object); // del just TOP level parent
		} else {	
			tmp=root->findChild(parent); // I want to remove something else - lower level
		}
		if (tmp == NULL ) {
			cout << "[ERROR] Wrong hierarchy" << endl; // there is no parrent
			return 1;
		} 
		if (child=="") { // on parent
			if (parent!="") {
				tmp=tmp->findChild(object);
			}
		} else {
				tmp = tmp->findChild(child); // I got child
		}
		if (tmp == NULL) {
				cout << "[ERROR] Wrong hierarchy." << endl;
				return 1;
		} else {
				tmp->removeMe(toRemove);
		}
	}
  string invokeProc="removeFromSingle {" + toRemove + "}";
  Tcl_Eval(interp, invokeProc.c_str());
  
	return TCL_OK;
}

int C_Tcl_interface::displayStructure (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

	string cmdArgs;
  if (1!=objc & 3 != objc) {
    cout << "[ERROR] Syntax error." << endl;
    return TCL_ERROR;
  } else {
    if (3 == objc) {
      cmdArgs=cmdArgs + " " + Tcl_GetString(objv[1]) + " " + Tcl_GetString(objv[2]);
    }
    string validateCmd="validateCmdLine_displayObject {" + cmdArgs + "}";
		if("TCL_OK" != run_Tcl_Eval(interp, validateCmd.c_str())) {
      return 1;
    }
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
	if (printStructure(interp, objc, objv, outputString,parent,child)) {
		return 1;
	} 
	cout<<outputString;
	return 0; 
}

int C_Tcl_interface::printStructure (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], string& outputString, const string& parent, const string& child){
	Object* tmp;
	if (parent=="") {
		root->printName(outputString);
	} else {
		tmp=root->findChild(parent);
		if (tmp == NULL ) {
			cout << "[ERROR] Wrong hierarchy." << endl;
			return 1;
		} else {
			if (child=="") {
				tmp->printName(outputString);
			} else {
				tmp = tmp->findChild(child);
				if (tmp == NULL) {
					cout << "[ERROR] Wrong hierarchy." << endl;
					return 1;
				} else {
					tmp->printName(outputString);
					}
				}
			}
		}
}

int C_Tcl_interface::writeFile (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
   
  if (root->child==NULL) { // Ugly 
		cout << "[ERROR] Nothing to save." << endl;
		return 1;
  }
  if (2 != objc) {
  	cout << "[ERROR] Syntax error." << endl;
    return TCL_ERROR;
  }
	string cmdArg=Tcl_GetString(objv[1]);
	string validateFile="writeFileCheck " + cmdArg;
	if("TCL_OK" == run_Tcl_Eval(interp,validateFile.c_str())) {
		string outputString="";
  	ofstream fh(Tcl_GetString(objv[1]));
    printStructure(interp, objc, objv, outputString, "", "");
		fh<<outputString;
		fh.close();
	}
}

int C_Tcl_interface::memoryUsage (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

   if (1 < objc) {
		cout << "[ERROR] Syntax error." << endl;
    return TCL_ERROR;
  }
  cout << "[Info] Memory usage is " << getValue() << endl;
  return TCL_OK;
}
