#include "interp.h"  

  
int main (int argc, char *argv[]) {

  C_Tcl_interface *inst = new C_Tcl_interface;
  
  cmdLineHandling (argc, *argv[]); // Parse cmdLine

  inst->tcl_main();
  
  return EXIT_SUCCESS;
}
