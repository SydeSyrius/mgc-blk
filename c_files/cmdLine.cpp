// Primitive CmdLine handling

class cmdLineMessage {
public:
	std::string toolName="TestingDojo";

	void printBanner();
	void printHelp();
	void printError();
};

void cmdLineMessage::printHelp() {
	cout << endl;
	cout << toolName << " [[-execute_script <string>] | [-help]]" << endl;
	cout << endl;
	exit (0);
}

void cmdLineMessage::printBanner() {
	cout << endl;
	cout << "#### " << toolName << " 0.1 ####" << endl;
	cout << "#########################" << endl;
	cout << endl;
}

void cmdLineMessage::printError() {
	cout << endl;
	cout << "[ERROR] Syntax error. Please use '" << toolName << " -help'." << endl;
	cout << endl;
	exit (1);
}
