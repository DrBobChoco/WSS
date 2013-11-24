// Group class functions
#include "classes.h"

Group::Group() {
	nextSquare = 0;
}

Group::Group(char initType) {
	if(initType == WSS_ROW || initType == WSS_COL || initType == WSS_BLOCK) gType = initType;
	else gType = 0;

	nextSquare = 0;
}

bool Group::setType(char newType) {
	if(!(newType == WSS_ROW || newType == WSS_COL || newType == WSS_BLOCK)) return false;
	
	gType = newType;
	return true;
}

bool Group::add(Square *newSquare) {
	if(nextSquare == WSS_GRP_MAX) return false;

	newSquare->setGroup(this, gType);
	squares[nextSquare++] = newSquare;
	return true;
}

bool Group::setValue(int square, int newVal) {
	cerr << "Setting square " << square << " to " << newVal << endl;
	return squares[square]->setValue(newVal);
}

int Group::getValue(int square) {
	return squares[square]->getValue();
}

list<int> Group::getPossibles(int square) {
	return squares[square]->getPossibles();
}

void Group::notPossible(int npVal) {
	for(int i=0; i<WSS_GRP_MAX; i++) {
		squares[i]->notPossible(npVal);
	}
}

void Group::notPossIfNotIn(int npVal, Group *group) {
	cerr << "notPossIfNotIn()!" << endl;
	for(int i=0; i<WSS_GRP_MAX; i++) {
		squares[i]->notPossIfNotIn(npVal, group);
	}
}

int Group::process() {
	int changes = 0;

	//any numbers only possible in one square?
	int possTally[WSS_GRP_MAX+1] = {0};

	for(int i=0; i<WSS_GRP_MAX; i++) {
		squares[i]->tallyPossibles(possTally);
	}

	for(int v=1; v<=WSS_GRP_MAX; v++) {
		if(possTally[v] == 1) {
			changes++;
			cerr << "Setting value [" << v << "] due to only being possible for 1 square" << endl;
			for(int i=0; i<WSS_GRP_MAX; i++) {
				squares[i]->setValue(v); //will only actually set for the one sqaure which can be this value
			}
		}
	}

	//any numbers only in one other group?
	set<Group *> possByGroup[WSS_GRP_MAX+1];

	if(gType == WSS_BLOCK) {
		for(int i=0; i<WSS_GRP_MAX; i++) {
			squares[i]->tallyPossByGroup(possByGroup, WSS_ROW);
		}
		for(int v=1; v<=WSS_GRP_MAX; v++) {
			if(possByGroup[v].size() == 1) {
				changes++;
				(*(possByGroup[v].begin()))->notPossIfNotIn(v, this);
			}
			possByGroup[v].clear();
		}

		for(int i=0; i<WSS_GRP_MAX; i++) {
			squares[i]->tallyPossByGroup(possByGroup, WSS_COL);
		}
		for(int v=1; v<=WSS_GRP_MAX; v++) {
			if(possByGroup[v].size() == 1) {
				changes++;
				(*(possByGroup[v].begin()))->notPossIfNotIn(v, this);
			}
		}
	} else {
		for(int i=0; i<WSS_GRP_MAX; i++) {
			squares[i]->tallyPossByGroup(possByGroup, WSS_BLOCK);
		}
		for(int v=1; v<=WSS_GRP_MAX; v++) {
			if(possByGroup[v].size() == 1) {
				changes++;
				(*(possByGroup[v].begin()))->notPossIfNotIn(v, this);
			}
		}
	}

	//identical n-sized possibles list
	// maybe have max passable by argument eventually?
	map<list<int>, int> possComp;

	for(int n=2; n<=3; n++) {
		possComp.clear();
		for(int i=0; i<WSS_GRP_MAX; i++) {
			squares[i]->tallyPossIfLength(&possComp, n);
		}

		map<list<int>, int>::iterator it;
		for(it = possComp.begin(); it != possComp.end(); it++) {

			cerr << "possComp[";
			list<int> tPoss = it->first;
			list<int>::iterator ii;
			for(ii = tPoss.begin(); ii != tPoss.end(); ii++) {
				cerr << *ii;
			}
			cerr << "] is " << it->second << endl;

			if(it->second == n) {
				changes++;
				cerr << "calling notPossIfPossNot()" << endl;
				for(int i=0; i<WSS_GRP_MAX; i++) {
					squares[i]->notPossIfPossNot(it->first);
				}
			}
		}
	}

	return changes;
}

void Group::printValues() {
	int val;
	cout << '|';
	for(int i=0; i<WSS_GRP_MAX; i++) {
		val = squares[i]->getValue();
		val ? cout << val : cout << " ";
		if(i%3 == 2) {
			cout << '|';
		}
	}
	cout << '\n';
	/*
	int nlEvery;
	char *init;
	char *every3;
	char *end;

	if(gType == WSS_COL) {
		nlEvery = 1;
		init = end = "-\n";
		every3 = "-\n";
	} else if(gType == WSS_BLOCK) {
		nlEvery = 0; //no regular newlines
		init = "+---+\n|";
		every3 = "|\n|";
		end = "
	} else {
	}
	*/
}
