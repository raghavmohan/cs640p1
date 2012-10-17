#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udp.h"
#include <arpa/inet.h>
#include "common.h"

#define DEBUG 1

//#include <netin/tcp.h>
//#include <netin/in.h>
typedef struct track_t{
	char* file;
	int id;
	char*  mac;
	int port;
}tracker;

typedef struct packArgs{
	char * hostname;
	int port;
}packArgs;

void getoptions (int argc, char **argv);

char * getHost(){
	char hostname[1024];
	hostname[1023] = '\0';
	gethostname(hostname, 1023);
	return strdup(hostname);
}

void usage(int terminate){
	printf("Usage requester -p <port> -o <fileoption>\n");
	if(terminate)
		exit(0);
}
typedef struct msg_t{
	char buffer[4096];
}msg_t;

int port;
char* fileOpt;
static struct sockaddr_in saddr;
//int sd;
void getoptions (int argc, char **argv) {
	int c;
	while ((c = getopt (argc, argv, "v?hRs:O:o:P:p:")) != -1){
		switch (c) {
			case 'h':
				usage(0);
				break;
			case 'p':
				port =atoi(optarg);
				break;
			case 'o':
				fileOpt = malloc(MAXLINESIZE);
				sprintf(fileOpt,  optarg);
				break;
			default:
				break;
		}
	}
} 

int numLines(char *filename)
{
	FILE *f;
	char c;
	int lines = 0;

	f = fopen(filename, "r");

	if(f == NULL)
		return -1;

	while((c = fgetc(f)) != EOF)
		if(c == '\n')
			lines++;

	fclose(f);

	return lines;
}
void printTrack(tracker tracktop){
	printf("track.file: %s\ntrack.id: %d\ntrack.mac: %s\ntrack.port: %d\n",tracktop.file, tracktop.id, tracktop.mac, tracktop.port); 
}
int getNumChunks(tracker * tracks, char * fileOpt, int trackLen){
	int i, numChunks;
	numChunks=0;
	for(i=0; i < trackLen; ++i){
		if(strcmp(tracks[i].file, fileOpt) == 0)
			++numChunks;
	}
	return numChunks;
}

packArgs getIDParams(tracker * tracks, char * fileOpt,int id, int trackLen){
	int i = 0;
	packArgs args;
	for(i; i < trackLen; ++i){
		if((strcmp(tracks[i].file, fileOpt) == 0) && (tracks[i].id == id)){
			args.hostname = tracks[i].mac;
			args.port = tracks[i].port;
		}
	}
	return arg;
}



