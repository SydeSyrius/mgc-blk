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
