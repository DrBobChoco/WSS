//Functions for the interactive bit
#include "wss.h"
#include <sstream>

void printGrid(Group *rows) {
	printLine();
	for(int row=0; row<WSS_GRP_MAX; row++) {
		rows[row].printValues();
		if(row%3 == 2) printLine();
	}
}

void printLine() {
	cout << "+---+---+---+\n";
}

void confirm(string cmd, Group *rows) {
	int row = 0, col = 0, val = 0;
	istringstream args(cmd);

	args >> row >> col >> val;
	if((row < 1 || row > 9) || (col < 1 || col > 9) || !val) {
		cout << "Incorrect arguments to 'confirm'" << endl;
		help(false);
		return;
	}
	
	cout << "The value for row " << row << ", column " << col << " is " << (val == rows[row-1].getValue(col-1) ? "" : "NOT ") << val << endl;
}

void tell(string cmd, Group *rows) {
	int row = 0, col = 0;
	istringstream args(cmd);

	args >> row >> col;
	if((row < 1 || row > 9) || (col < 1 || col > 9)) {
		cout << "Incorrect arguments to 'tell'" << endl;
		help(false);
		return;
	}
	
	cout << "The value for row " << row << ", column " << col << " is " << rows[row-1].getValue(col-1) << endl;
}

void showPossibles(string cmd, Group *rows) {
	int row = 0, col = 0;
	istringstream args(cmd);

	args >> row >> col;
	if((row < 1 || row > 9) || (col < 1 || col > 9)) {
		cout << "Incorrect arguments to 'poss'" << endl;
		help(false);
		return;
	}
	
	cout << "Row " << row << ", column " << col << " possibles: [";
	list<int> possibles = rows[row-1].getPossibles(col-1);
	if(possibles.empty()) {
		cout << "NONE";
	} else {
		bool comma = false;
		list<int>::iterator it;
		for(it = possibles.begin(); it != possibles.end(); it++) {
			if(comma) {
				cout << ",";
			} else {
				comma = true;
			}
			cout << *it;
		}
	}
	cout << "]" << endl;
}

void help(bool showCLHelp) {
	if(showCLHelp) {
		cout << "Usage: wss [OPTION]\n";
		cout << "<no option>            prompt for initial values interactively\n";
		cout << "-f, --file <file name> read initial values from file\n";
		cout << "                       initial values should be on one row of text per row with\n";
		cout << "                       unknown values replaced by '.' or ' '\n";
		cout << "-d, --debug            print debug information\n";
		cout << "-?, --help             show this message\n" << endl;
	}
	cout << "Interactive commands:\n";
	cout << " show, print    print the completed grid\n";
	cout << " tell r c       tell the value of the square at row r, column c\n";
	cout << " confirm r c v  confirm that the square at row r, column c has the value v\n";
	cout << "                [row and column numbers start from 1]\n";
	cout << " quit, exit     exit wss\n";
	cout << " ?, help        show this message" << endl;
}
