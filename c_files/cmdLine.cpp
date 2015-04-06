// Primitive CmdLine handling

class cmdLineMessage {
public:
	std::string toolName="testingDojo";

	void printBanner();
	void printHelp();
	void printError();
};

void cmdLineMessage::printHelp() {
	cout << endl;
	cout << toolName << "				-> interactive mode" << endl;
	cout << toolName << " -execute_script <string>	-> interactive mode with automated script execution" << endl;
	cout << toolName << " -help		 	-> display help" << endl;
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
	cout << "[ERROR] Syntax error - try testingDojo -help" << endl;
	cout << endl;
	exit (1);
}
