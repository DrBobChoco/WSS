// Solver classes
#include <iostream>
#include <list>
#include <map>
#include <set>

using namespace std;

#define WSS_ROW 1
#define WSS_COL 2
#define WSS_BLOCK 3

#define WSS_GRP_MAX 9

class Group;

class Square {
	int       value;
	list<int> possibles;
	Group     *myRow;
	Group     *myCol;
	Group     *myBlock;

	public:
		Square();
		list<int> getPossibles();
		int       getValue();
		bool      isPossibly(int);
		void      notPossible(int);
		void      notPossIfNotIn(int, Group *);
		void      notPossIfPossNot(list<int>);
		bool      setGroup(Group *, char);
		bool      setValue(int);
		void      tallyPossByGroup(set<Group *> *, char);
		void      tallyPossIfLength(map<list<int>, int> *, int);
		void      tallyPossibles(int *);
};

class Group {
	char    gType;
	Square  *squares[WSS_GRP_MAX];
	int     nextSquare;

	public:
		Group();
		Group(char);
		bool      add(Square *);
		list<int> getPossibles(int);
		int       getValue(int);
		void      notPossible(int);
		void      notPossIfNotIn(int, Group *);
		void      printValues();
		int       process();
		bool      setType(char);
		bool      setValue(int, int);
};
