#include "interp.h"  

  
int main (int argc, char *argv[]) {

  C_Tcl_interface *inst = new C_Tcl_interface;
  
  inst->tcl_main(argc, argv);

  return EXIT_SUCCESS;
}
