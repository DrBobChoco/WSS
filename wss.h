#include "classes.h"
#include <getopt.h>
#include <iostream>
#include <signal.h>

#define WSS_VALS_FROM_CL   0
#define WSS_VALS_FROM_FILE 1

void confirm(string, Group *);
void help(bool);
void printGrid(Group *);
void printLine();
void showPossibles(string, Group *);
void tell(string, Group *);
