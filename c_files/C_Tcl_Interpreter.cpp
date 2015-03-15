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
    int readInputDofile (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int execute_plus (int ,Tcl_Obj *CONST objv[]);
    int execute_plus_in_Tcl (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int execute_plus_plus_in_Tcl (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int addToClist (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int removeFromClist (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int wyswietl_strukture(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);

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

  // wait for commands promt
 // while (1) {
    char cmd[1024];
    //fgets (cmd, sizeof (cmd), stdin);
    string cmdd = "source tcl_files/inputParser.tcl\nsource Tcl_int.tcl";
    cmdLineHandling (argc, argv);
    if (TCL_OK != Tcl_Eval (interp, cmdd.c_str())) {
      cout<<"Error: "<<Tcl_GetStringResult (interp)<<endl;
    }
  //}
}

void C_Tcl_interface::InitializeCommand(string command_name) {
  Tcl_CreateObjCommand(interp,command_name.c_str(), AddObjCmd, (ClientData) this, (Tcl_CmdDeleteProc *) NULL);
}

void C_Tcl_interface::InitializeCommands() {
  // add your cammand name here to be callable from Tcl shell
  InitializeCommand("+");
  InitializeCommand("+tcl");
  InitializeCommand("+tcltcl");
  
  InitializeCommand("readInputDofile");
  InitializeCommand("addToClist");
  InitializeCommand("removeFromClist");
  InitializeCommand("wyswietl_strukture");
}

int C_Tcl_interface::LinkCommand (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    // This is a command parser
    
    
    // objc                    -> number of arguments    
    // objv[0]                 -> command name
    // objv[1] to objv[objc-1] -> arguments
    string commandName = Tcl_GetString(objv[0]);
    //cout<<"COM:"<<commandName<<endl;
    
    if (commandName=="+") {
      return execute_plus(objc, objv);
    } else if (commandName=="+tcl") {
      return execute_plus_in_Tcl(interp, objc, objv) ;
    } else if (commandName=="+tcltcl") {
      return execute_plus_plus_in_Tcl(interp, objc, objv) ;
    } else if (commandName=="addToClist") {
      return addToClist(interp, objc, objv) ;
    } else if (commandName=="removeFromClist") {
        return removeFromClist(interp, objc, objv) ;
    } else if (commandName=="wyswietl_strukture") {
      return wyswietl_strukture(interp, objc, objv) ;
    } else if (commandName=="readInputDofile") {
       return readInputDofile(interp, objc, objv) ;
    }

  
  return TCL_OK;
}

// This method is parsing arguments from Tcl interpreted and calculating result in C
int C_Tcl_interface::execute_plus(int objc, Tcl_Obj *CONST objv[]) {
  long n1,n2;
  
  if (3 != objc) {
    Tcl_WrongNumArgs (interp, 1, objv, "n1 n2");
    return TCL_ERROR;
  }

  if (TCL_OK != Tcl_GetLongFromObj (interp, objv[1], &n1)) {
    return TCL_ERROR;
  }

  if (TCL_OK != Tcl_GetLongFromObj (interp, objv[2], &n2)) {
    return TCL_ERROR;
  }

  // 
  
  cout<<"Results is: "<< n1 + n2 <<" ."<<endl;
  return TCL_OK;
}

// This method is linking values in Tcl and calculating results in tcl
int C_Tcl_interface::execute_plus_in_Tcl(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
  long n1,n2;
  
  if (3 != objc) {
    Tcl_WrongNumArgs (interp, 1, objv, "n1 n2");
    return TCL_ERROR;
  }
  
  if (TCL_OK != Tcl_GetLongFromObj (interp, objv[1], &n1)) {
    return TCL_ERROR;
  }

  if (TCL_OK != Tcl_GetLongFromObj (interp, objv[2], &n2)) {
    return TCL_ERROR;
  } 

  
  Tcl_Obj *result;
  result = Tcl_NewLongObj (n1 + n2);
  Tcl_SetObjResult(interp, result);
  cout<<"Results: "<<Tcl_GetStringResult (interp)<<endl;
}

int C_Tcl_interface::readInputDofile(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

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

// This method is creating string in C, that is used to be a command in Tcl_CmdDeleteProc
int C_Tcl_interface::execute_plus_plus_in_Tcl(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
  long n1,n2;
  
  if (3 != objc) {
    Tcl_WrongNumArgs (interp, 1, objv, "n1 n2");
    return TCL_ERROR;
  }

  if (TCL_OK != Tcl_GetLongFromObj (interp, objv[1], &n1)) {
    return TCL_ERROR;
  }

  if (TCL_OK != Tcl_GetLongFromObj (interp, objv[2], &n2)) {
    return TCL_ERROR;
  } 
  
  
  Tcl_Obj *result;
  // string command = "puts [expr " + n1 + " " + n2 + " ] ";
  stringstream new_stream;
  string command ;
   
  // create string line 
  new_stream << "puts \"Resutsl : [expr " << n1 << " + " << n2 << " ] \" ";
  command = new_stream.str();

  Tcl_Eval(interp, command.c_str());
  return TCL_OK;
}

int C_Tcl_interface::addToClist(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
  string row;

   if (2 != objc) {
    Tcl_WrongNumArgs (interp, 1, objv, "n1 n2");
    return TCL_ERROR;
  }

  row = Tcl_GetString (objv[1]);
  data_structure->add_new_object(row);
  

//  Tcl_Eval(interp, command.c_str());
  return TCL_OK;
}

int C_Tcl_interface::removeFromClist(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	// del object
	return TCL_OK;

}
int C_Tcl_interface::wyswietl_strukture (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

   if (1 < objc) {
     // do poprawy
    //Tcl_WrongArgs (interp, 1, objv, "Ta opcja nie przyjmuje zadnych argumentow.");
    return TCL_ERROR;
  }

  
  data_structure->display_data();
  

//  Tcl_Eval(interp, command.c_str());
  return TCL_OK;
}
