// #include <stdlib.h>
// #include <string.h>
#include "getopt.h"
//#include "defs.h"

int optind=1;	// index of the next element to be processed in argv
int new_arg=1;
int next_char=0;
int no_option_fields=0;
char *place;

int
getopt (int nargc, char * const *nargv, const char *options) {
	//printf(1, "place:%c\n", *place);
	int optchar;
	if (optind>=nargc) {	// all arguments parsed
		return (-1);
	}

	if (new_arg) {
		place=nargv[optind];
		new_arg=0;
	}
	
	if (next_char==0) {
		if (*place!='-') {	// its a non-option
			new_arg=1;
			optind++;
		}
		else {
			place++;
			next_char++;
			no_option_fields++;
		}

		return(-2);
	}

	/*
	if (place[1]=='-') {	// if '--' found, stop parsing
		//optind++;	// is this required?
		return(-1);
	}
	*/

	optchar=*place++;
	next_char++;
	if (optchar=='\0') {
		new_arg=1;	// proceed to the next argument in the next iteration
		next_char=0;
		optind++;
		return -2;
	}
	return(optchar);
}