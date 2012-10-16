#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>

#include <unistd.h>

#define MAXLINESIZE 256
void errorExit(char * errorMessage){
	fprintf(stderr, "Error: %s\n", errorMessage);
	exit(1);
}


