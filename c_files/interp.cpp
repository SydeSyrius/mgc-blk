#include "interp.h"  


int main (int argc, char *argv[]) {

//  C_Tcl_interface *inst = new C_Tcl_interface;
  
//  inst->tcl_main(argc, argv);  
  /// add D -below a/z
  //  D  a  z
  // 
  data_container_new* lol = new data_container_new();
  lol->add_new_object("asd","","");
  lol->add_new_object("-as32d","","");
  lol->add_new_object("wae","","");
  lol->add_new_object("-as4d","","");
  lol->add_new_object("as32d","","");
  lol->add_new_object("-yas32d","","");
  lol->add_new_object("--tyas32d","","");
  lol->add_new_object("-yd","","");
  lol->add_new_object("q2d","","");
  lol->display_data();
 
  return EXIT_SUCCESS;
}
