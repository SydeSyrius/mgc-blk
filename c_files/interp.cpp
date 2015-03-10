#include "interp.h"  

  
int main (int argc, char *argv[]) {

  C_Tcl_interface *inst = new C_Tcl_interface;
  
  // Primitive CmdLine handling
  if (argc == 1) {
	cout << endl;
  	cout << "#### Testing Dojo wersja 0.1 ###" << endl;
  	cout << "################################" << endl;
  	cout << endl;
  }
  else if (argv[1] == std::string("-help")) { // Display help and exit
  	if (argc > 2) {
  		cout << "Blad - zobacz -help" << endl;
  		return 1;
  	}
  	else {
  		cout << endl;
  		cout << "#### Testing Dojo wersja 0.1 ###" << endl;
  		cout << "################################" << endl;

  		cout << "<testingDojo> 			-> uruchomienie w trybie interaktywnym" << endl;
  		cout << "<testingDojo> -skrypt <nazwa> 	-> uruchomienie w trybie interaktywnym z automatycznym startem" << endl;
  		cout << "<testingDojo> -help 		-> wyswietla pomoc" << endl;
  		cout << endl;

  		return 0;
  	}
  }
  else if (argv[1] == std::string("-skrypt")) {
  	if (argv[2] != NULL) {
  		if (argc>3){
  			cout << "Blad - sprawdz -help" << endl;
  			return 1;
  		}
  		else {
  			cout << "Wywolam " << argv[2];
  		}
  	}
   	else {
  		cout << "Blad" << endl;
  		return 1;
  	}
  }
  else {
  	cout<< "Niedozwolone komenda" << endl; // validate if this is correct
  	return 1;
  }

  inst->tcl_main();
  
  return EXIT_SUCCESS;
}
