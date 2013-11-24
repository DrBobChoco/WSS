// Square class functions
#include "classes.h"

Square::Square() {
	value = 0;
	for(int i=1; i<=WSS_GRP_MAX; i++) {
		possibles.push_back(i);
	}
}

bool Square::setGroup(Group *group, char type) {
	bool ret = false;

	switch(type) {
		case WSS_ROW:
			if(!myRow) {
				myRow = group;
				ret = true;
			}
			break;
		case WSS_COL:
			if(!myCol) {
				myCol = group;
				ret = true;
			}
			break;
		case WSS_BLOCK:
			if(!myBlock) {
				myBlock = group;
				ret = true;
			}
			break;
	}

	return ret;
}

bool Square::setValue(int newVal) {
	if(newVal < 1 || newVal > 9 || !isPossibly(newVal)) {
		cerr << "square rejecting value " << newVal << endl;
		return false;
	}

	cerr << "square setting value to " << newVal << endl;

	value = newVal;
	possibles.clear();

	myRow->notPossible(newVal);
	myCol->notPossible(newVal);
	myBlock->notPossible(newVal);
	return true;
}

int Square::getValue() {
	return value;
}

list<int> Square::getPossibles() {
	list<int> poss = possibles;
	return poss;
}

void Square::notPossible(int npVal) {
	if(possibles.empty()) return;

	//cerr << "square removing '" << npVal << "' from possibles list" << endl;

	possibles.remove(npVal);
	if(possibles.size() == 1) {
		cerr << "Setting value as only 1 possible left" << endl;
		setValue(possibles.front());
	}
}

void Square::notPossIfNotIn(int npVal, Group *group) {
	if(myRow != group && myCol != group && myBlock != group)
		notPossible(npVal);
}

void Square::notPossIfPossNot(list<int> testPoss) {
	cerr << "notPossIfPossNot()" << endl;

	if(possibles != testPoss) {
		list<int>::iterator it;
		for(it = testPoss.begin(); it != testPoss.end(); it++) {
			notPossible(*it);
		}
	}
}

bool Square::isPossibly(int val) {
	list<int>::iterator it;
	//cerr << "possibles currently has " << possibles.size() << " items in" << endl;
	for(it = possibles.begin(); it != possibles.end(); it++) {
		//cerr << "possibles iterator pointing to " << *it << endl;
		if(*it == val) return true;
	}

	return false;
}

void Square::tallyPossibles(int *possTally) {
	for(int i=1; i<=WSS_GRP_MAX; i++) {
		if(isPossibly(i)) {
			possTally[i]++;
		}
	}
}

void Square::tallyPossByGroup(set<Group *> *groupTally, char gType) {
	Group *tGroup;
	if(gType == WSS_ROW) {
		tGroup = myRow;
	} else if(gType == WSS_COL) {
		tGroup = myCol;
	} else if(gType == WSS_BLOCK) {
		tGroup = myBlock;
	} else {
		return;
	}

	for(int i=1; i<=WSS_GRP_MAX; i++) {
		if(isPossibly(i)) {
			groupTally[i].insert(tGroup);
		}
	}
}

void Square::tallyPossIfLength(map<list<int>, int> *possComp, int n) {
	if(possibles.size() == n) {
		(*possComp)[possibles] = (*possComp)[possibles] + 1;
	}
}
