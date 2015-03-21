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
    data_container *data_structure;
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

    // Registered cmds
    int readInputDoFile (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int addToClist (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int removeFromClist (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
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
  
  data_structure = new data_container();
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
  } else if (argv[1] == std::string("-skrypt")) {
    if (argv[2] != NULL) {
      if (argc>3){
	message.printError();
      } else {
	message.printBanner();
	cmd = "source tcl_files/tcl_init_variables.tcl\nwykonaj_skrypt " + std::string(argv[2]) + "\nsource Tcl_int.tcl";
      }
    } else {
      message.printError();
    }
  } else {
    message.printError();
  }
  // Run tcl console
  if (TCL_OK != Tcl_Eval (interp, cmd.c_str())) {
    cout<<"Error: "<<Tcl_GetStringResult (interp)<<endl;
    exit (1);
  }
  return 0;
}

void C_Tcl_interface::InitializeCommand(string command_name) {
  Tcl_CreateObjCommand(interp,command_name.c_str(), AddObjCmd, (ClientData) this, (Tcl_CmdDeleteProc *) NULL);
}

void C_Tcl_interface::InitializeCommands() {
  // add your cammand name here to be callable from Tcl shell
  InitializeCommand("wykonaj_skrypt");
  InitializeCommand("dodaj_obiekt");
  InitializeCommand("usun_obiekt");
  InitializeCommand("raportuj_obiekt");
  InitializeCommand("zapisz_plik_wynikowy");
  InitializeCommand("memory_usage");
}

int C_Tcl_interface::LinkCommand (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    // This is a command parser
    // objc                    -> number of arguments    
    // objv[0]                 -> command name
    // objv[1] to objv[objc-1] -> arguments
    string commandName = Tcl_GetString(objv[0]);
    
    if (commandName=="dodaj_obiekt") {
      return addToClist(interp, objc, objv) ;
    } else if (commandName=="usun_obiekt") {
        return removeFromClist(interp, objc, objv) ;
    } else if (commandName=="raportuj_obiekt") {
      return displayStructure(interp, objc, objv) ;
    } else if (commandName=="zapisz_plik_wynikowy") {
      return writeFile(interp, objc, objv) ;
    } else if (commandName=="wykonaj_skrypt") {
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

  cout<<"Wykonuje skrypt "<<  Tcl_GetString(objv[1]) << endl ;
  if (TCL_OK != Tcl_EvalFile(interp,Tcl_GetString(objv[1]))) {
	  return TCL_ERROR;
  }
  return 0;
}

int C_Tcl_interface::addToClist(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
  string row;

   if (2 != objc) {
    Tcl_WrongNumArgs (interp, 1, objv, "n1 n2");
    return TCL_ERROR;
  }

  row = Tcl_GetString (objv[1]);
  // Add to tcl list first
  string invokeProc="add_to_single_name " + row; 
  Tcl_Eval(interp, invokeProc.c_str());
  // Add to list
  data_structure->add_new_object(row);
  

//  Tcl_Eval(interp, command.c_str());
  return TCL_OK;
}

int C_Tcl_interface::removeFromClist(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	// del object
  string row;

   if (2 != objc) {
    Tcl_WrongNumArgs (interp, 1, objv, "object name");
    return TCL_ERROR;
  }
  
  row=Tcl_GetString (objv[1]);
  string invokeProc="removeFromSingle " + row;
  Tcl_Eval(interp, invokeProc.c_str());

  data_structure->delete_object(row);

  return TCL_OK;
}
int C_Tcl_interface::displayStructure (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

   if (1 < objc) {
     // do poprawy
    //Tcl_WrongArgs (interp, 1, objv, "Ta opcja nie przyjmuje zadnych argumentow.");
    return TCL_ERROR;
  }
  cout << data_structure->display_data_new();
  //data_structure->display_data();
  return TCL_OK;
}

int C_Tcl_interface::memoryUsage (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

   if (1 < objc) {
     // do poprawy
    //Tcl_WrongArgs (interp, 1, objv, "Ta opcja nie przyjmuje zadnych argumentow.");
    return TCL_ERROR;
  }
  cout << "Uzycie pamieci " << getValue();
  return TCL_OK;
}

int C_Tcl_interface::writeFile (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
   
  if (2 != objc) {
          Tcl_WrongNumArgs (interp, 1, objv, "<file_name>");
          return TCL_ERROR;
  }
  ofstream fh;
  fh.open(Tcl_GetString(objv[1]));
  fh<<data_structure->display_data_new();
  fh.close();
}
