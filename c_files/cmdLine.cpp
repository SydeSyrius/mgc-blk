// Primitive CmdLine handling

class cmdLineMessage {
public:
	std::string toolName="KriBaL";

	void printBanner();
	void printHelp();
	void printError();
};

void cmdLineMessage::printHelp() {
	cout << endl;
	cout << toolName << " 					-> uruchomienie w trybie interaktywnym" << endl;
	cout << toolName << "-skrypt <nazwa> 	-> uruchomienie w trybie interaktywnym z automatycznym startem" << endl;
	cout << toolName << "-help 				-> wyswietla pomoc" << endl;
	cout << endl;
	exit (0);
}

void cmdLineMessage::printBanner() {
	cout << endl;
	cout << "#### " << toolName << " 0.1 ####" << endl;
	cout << "####################" << endl;
	cout << endl;
}

void cmdLineMessage::printError() {
	cout << endl;
	cout << "Blad - zobacz -help" << endl;
	cout << endl;
	exit (1);
}

int C_Tcl_interface::cmdLineHandling (int argc, char *argv[]) {
	  cmdLineMessage message;
	  if (argc == 1) {
		  message.printBanner();
	  }
	  else if (argv[1] == std::string("-help")) {
		if (argc > 2) {
			message.printError();
		}
		else {
			message.printHelp();
		}
	  }
	  else if (argv[1] == std::string("-skrypt")) {
		if (argv[2] != NULL) {
			if (argc>3){
				message.printError();
			}
			else {
				message.printBanner();
				string cmd = "wykonaj_skrypt " + std::string(argv[2]);
				if (TCL_OK != Tcl_Eval (interp, cmd.c_str())) {
				      cout<<"Error: "<<Tcl_GetStringResult (interp)<<endl;
				      exit (1);
				}


			}
		}
		else {
			message.printError();
		}
	  }
	  else {
			message.printError();
	  }
return 0;
}
