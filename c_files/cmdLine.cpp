// Primitive CmdLine handling

class cmdLineMessage {
public:
	void printBanner();
	void printHelp();
	void printError();
};

void cmdLineMessage::printHelp() {
	cout << endl;
	cout << "<testingDojo> 					-> uruchomienie w trybie interaktywnym" << endl;
	cout << "<testingDojo> -skrypt <nazwa> 	-> uruchomienie w trybie interaktywnym z automatycznym startem" << endl;
	cout << "<testingDojo> -help 			-> wyswietla pomoc" << endl;
	cout << endl;
}

void cmdLineMessage::printBanner() {
	cout << endl;
	cout << "#### Testing Dojo wersja 0.1 ###" << endl;
	cout << "################################" << endl;
	cout << endl;
}

void cmdLineMessage::printError() {
	cout << endl;
	cout << "Blad - zobacz -help" << endl;
	cout << endl;
}

int cmdLineHandling (int argc, char *argv[]) {
	  cmdLineMessage message = new cmdLineMessage ();
	  if (argc == 1) {
		  message.printBanner();
	  }
	  else if (argv[1] == std::string("-help")) {
		if (argc > 2) {
			message.printError();
			return 1;
		}
		else {
			message.printHelp();
			return 0;
		}
	  }
	  else if (argv[1] == std::string("-skrypt")) {
		if (argv[2] != NULL) {
			if (argc>3){
				message.printError();
				return 1;
			}
			else {
				string cmd = "wczytaj_plik " + argv[2];
				Tcl_Eval(interp, cmd.c_str() ) ;
				return 0;
			}
		}
		else {
			message.printError();
			return 1;
		}
	  }
	  else {
			message.printError();
			return 1;
	  }
return 0;
}
