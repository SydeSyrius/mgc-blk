/*
 * C_Tcl_interface.cpp
 * 
 * 
 * 
 * 
 */

class C_Tcl_interface {
    public:
    Tcl_Interp *interp;  
    int argss_n; 
    
    C_Tcl_interface();
    static int AddObjCmd (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
      return (reinterpret_cast<C_Tcl_interface*>(clientData))->LinkCommand (interp, objc, objv);
    }
    int LinkCommand (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    void tcl_main();
    void InitializeCommand(string) ;
    void InitializeCommands() ;
    int execute_plus (int ,Tcl_Obj *CONST objv[]);
    int execute_plus_in_Tcl (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
    int execute_plus_plus_in_Tcl (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
  
};


C_Tcl_interface :: C_Tcl_interface () {
//  Tcl_FindExecutable
  interp = Tcl_CreateInterp();
  if (TCL_OK != Tcl_Init (interp)) {
    fprintf (stderr, "Tcl_Init error: %s\n", Tcl_GetStringResult (interp));
    exit (EXIT_FAILURE);
  }
}

void C_Tcl_interface::tcl_main() {
  
  // Link Commands commands
  InitializeCommands();
  
  // wait for commands promt
 // while (1) {
    char cmd[1024];
    //fgets (cmd, sizeof (cmd), stdin);
    string cmdd = "source Tcl_int.tcl";
    if (TCL_OK != Tcl_Eval (interp, cmdd.c_str())) {
      cout<<"Error: "<<Tcl_GetStringResult (interp)<<endl;
    //  continue;
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
  
}

int C_Tcl_interface::LinkCommand (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    // This is a command parser
    
    
    // objc                    -> number of arguments    
    // objv[0]                 -> command name
    // objv[1] to objv[objc-1] -> arguments
    string commandName = Tcl_GetString(objv[0]);
    
    if (commandName=="+") {
      return execute_plus(objc, objv);
    } else if (commandName=="+tcl") {
      return execute_plus_in_Tcl(interp, objc, objv) ;
    } else if (commandName=="+tcltcl") {
      return execute_plus_plus_in_Tcl(interp, objc, objv) ;
    }
    
  
//    string test;
//    Tcl_Obj *res;
//    res = Tcl_NewLongObj (n1 + n2);
//    Tcl_SetObjResult (interp, res);
//    cout<<"result is  : "<< Tcl_GetStringResult (interp)<<endl; 
//    test = "set a a";
  //  Tcl_Eval(interp, test.c_str());
       
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