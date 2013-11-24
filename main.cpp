#include "wss.h"
#include <fstream>
#include <sstream>

int  *extractRowVals(string);
void  redoCerr();
void  sigCatch(int);
int **valsFromCL();
int **valsFromFile(const char *);

basic_streambuf<char> *oldCerr = NULL;

main(int argc, char **argv) {
	Group  rows[WSS_GRP_MAX];
	Group  cols[WSS_GRP_MAX];
	Group  blocks[WSS_GRP_MAX];

	for(int i=0; i<WSS_GRP_MAX; i++) {
		rows[i].setType(WSS_ROW);
		cols[i].setType(WSS_COL);
		blocks[i].setType(WSS_BLOCK);
	}

	/*
	map<const char *, int> testMap;
	testMap["blah"] = 2;
	cout << "blah : " << testMap["blah"] << "\nwhoo inc : " << ++testMap["whoo"] << "\n whoo : " << testMap["whoo"] << endl;
	return 0;
	*/

	//create squares and add to groups
	int    block;
	for(int row=0; row<WSS_GRP_MAX; row++) {
		for(int col=0; col<WSS_GRP_MAX; col++) {
			Square *newSq = new Square;
			rows[row].add(newSq);
			cols[col].add(newSq);
			block = (int)(col/3) + 3*((int)(row/3));
			blocks[block].add(newSq);
		}
	}

	//get opts
	opterr = 0;
	struct option longOpts[] = {
		{"file", required_argument, NULL, 'f'},
		{"debug", no_argument, NULL, 'd'},
		{"help", no_argument, NULL, '?'},
		{0, 0, 0, 0}
	};
	int getValsHow = WSS_VALS_FROM_CL;
	bool showDebug = false;
	string fName;

	int opt;
	while((opt = getopt_long(argc, argv, "f:d?", longOpts, NULL)) != -1) {
		switch(opt) {
			case 'f':
				getValsHow = WSS_VALS_FROM_FILE;
				fName = optarg;
				break;
			case 'd':
				showDebug = true;
				break;
			case '?':
				help(true);
				exit(1);
		}
	}

	if(optind == argc - 1 && fName == "") {
		getValsHow = WSS_VALS_FROM_FILE;
		fName = argv[optind];
	}

	if(!showDebug) {
		signal(SIGINT, sigCatch);
		stringbuf newCerr(ios_base::in);
		oldCerr = cerr.rdbuf(&newCerr);
	}

	//set values
	int **vals;
	if(getValsHow == WSS_VALS_FROM_FILE) {
		vals = valsFromFile(fName.c_str());
	} else {
		vals = valsFromCL();
	}

	for(int row=0; row<WSS_GRP_MAX; row++) {
		for(int col=0; col<WSS_GRP_MAX; col++) {
			rows[row].setValue(col, vals[row][col]);
		}
	}

	int changes;
	int cycles=0;
	do {
		cerr << "Doing a process cycle..." << endl;
		changes = 0;
		for(int i=0; i<WSS_GRP_MAX; i++) {
			changes += rows[i].process();
		}
		for(int i=0; i<WSS_GRP_MAX; i++) {
			changes += cols[i].process();
		}
		for(int i=0; i<WSS_GRP_MAX; i++) {
			changes += blocks[i].process();
		}

		cycles++;
	} while(changes && cycles < 20);

	cout << "Ready..." << endl;
	string cmd;
	bool quit = false;
	do {
		cout << "wss: ";
		getline(cin, cmd);

		if(cmd == "?" || cmd == "help") {
			help(false);
		} else if(cmd == "show" || cmd == "print") {
			printGrid(rows);
		} else if(cmd.compare(0, 5, "tell ") == 0 && cmd.length() == 8) {
			cmd.erase(0, 5);
			tell(cmd, rows);
		} else if(cmd.compare(0, 8, "confirm ") == 0 && cmd.length() == 13) {
			cmd.erase(0, 8);
			confirm(cmd, rows);
		} else if(cmd.compare(0, 5, "poss ") == 0 && cmd.length() == 8) {
			cmd.erase(0, 5);
			showPossibles(cmd, rows);
		} else if(cmd == "quit" || cmd == "q" || cmd == "exit") {
			quit = true;
		} else {
			if(cmd != "") {
				cout << "Unrecognised command" << endl;
				help(false);
			}
		}
	} while(!quit);
}

int **valsFromCL() {
	int **vals = (int **)malloc(WSS_GRP_MAX * sizeof(int *));
	string rowIn;

	for(int row=0; row<WSS_GRP_MAX; row++) {
		cout << "Starting values for row " << row+1 << ":" << endl;
		getline(cin, rowIn);
		cerr << "Got row '" << rowIn << "'" << endl;
		while(rowIn.length() != WSS_GRP_MAX) {
			cout << "Wrong number of values. Try again:" << endl;
			getline(cin, rowIn);
		}
		vals[row] = extractRowVals(rowIn);
	}

	return vals;
}

int **valsFromFile(const char *fName) {
	int **vals = (int **)malloc(WSS_GRP_MAX * sizeof(int *));
	string rowIn;
	ifstream inFile(fName);
	//File = new ifstream(fName);
	
	if(inFile.fail()) {
		cerr << "Failed to open '" << fName << "'" << endl;
		exit(1);
	}

	for(int row=0; row<WSS_GRP_MAX; row++) {
		getline(inFile, rowIn);
		if(rowIn.length() != WSS_GRP_MAX) {
			cerr << "Row " << row+1 << " is wrong length" << endl;
			exit(1);
		}
		vals[row] = extractRowVals(rowIn);
	}
	inFile.close();

	return vals;
}

int *extractRowVals(string rowIn) {
	int *rowVals = (int *)malloc(WSS_GRP_MAX * sizeof(int));
	char valBuf[2];
	valBuf[1] = '\0';

	cerr << "Getting values" << endl;
	for(int col=0; col<WSS_GRP_MAX; col++) {
		valBuf[0] = rowIn.at(col);
		cerr << "valBuf '" << valBuf << "'" << endl << "...which is:" << atoi(valBuf) << endl;
		rowVals[col] = atoi(valBuf);
	}

	return rowVals;
}

void redoCerr() {
	if(oldCerr) {
		cerr.rdbuf(oldCerr);
	}
}

void sigCatch(int sig) {
	redoCerr();
	exit(1);
}
