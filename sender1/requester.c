#include "requester.h"
int main (int argc, char ** argv){
	if(argc != 5)
		usage(1);

	getoptions(argc, argv);

	if(DEBUG){
		printf("port: %d\n", port);
		printf("fileOpt: %s\n", fileOpt);
	}

	int fd, bytesRead, fileSize, counter, i;

	char* inFile = "./tracker.txt";


	FILE * trackFp =fopen(inFile, "r");
	if(trackFp == NULL)	
		errorExit("bad Tracker");

	int numlines = numLines(inFile);
	if(DEBUG)
		printf("Numlines:%d\n", numlines);

	tracker * tracks = malloc ( numlines * sizeof(tracker));
	buildTracker(tracks, trackFp);
	//sort the tracks to make easier for later. Look @ compare to see sort order
	qsort (tracks, numlines, sizeof(tracks[0]), compare);
	if(DEBUG){
		for(i =0; i < numlines ; ++i)
			printTrack(tracks[i]);
	}

	//now we have all information in memory
	int numChunks = getNumChunks(tracks, fileOpt, numlines);
	printf("numChunks:%d\n", numChunks);
	for(i=1; i <= numChunks; ++i){
		packArgs argsP = getIDParams(tracks, fileOpt, i, numlines);
		if(DEBUG > 1)
			printf("host:%s, port:%d\n", argsP.hostname, argsP.port);

		getPackets(argsP.hostname, argsP.port);
	}

	return 0;
}

/*
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

struct sockaddr_in saddr;
int sd;
int port;
struct timeval tv;
int retval;
fd_set rfds;
char * fileOpt;

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
				fileOpt = strdup(optarg);
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
	return args;
}





int buildTracker(tracker * tracks, FILE * trackFp){
	char linebuf[MAXLINESIZE];	
	int i =0, counter =0;
	while(fgets(&linebuf, MAXLINESIZE, trackFp) > 0){
		char * pch;
		printf ("Splitting string \"%s\" into tokens:\n",linebuf);
		pch = strtok (linebuf," \n");
		counter = 0;
		while (pch != NULL)
		{
			switch(counter){
				case 0:
					tracks[i].file = strdup(pch);	
					break;
				case 1:
					tracks[i].id= atoi(pch);	
					break;
				case 2:
					tracks[i].mac = strdup(pch);	
					break;
				case 3:
					tracks[i].port = atoi(pch);
					break;
				default:
					break;
			}
			++counter;
			//			printf ("%s",pch);
			pch = strtok (NULL, " \n");
		}
		++i;
	}

	return 1;
}

int compare(const void *p1, const void *p2)
{
	const tracker *elem1 = p1;    
	const tracker *elem2 = p2;
	//take care of interesting case first

	if( strcmp(elem1->file, elem2->file) == 0){
		if(elem1->id > elem2->id)
			return 1;
		else if(elem1->id < elem2->id)
			return -1;
		else
			return 0;
	}
	else{
		return ( strcmp(elem1->file, elem2->file));
	}
}
//This get from the TRACKER txt
int  getPackets(char * hostname, int portIn){
	sd = UDP_Open(-1);
	assert(sd > -1);


	//This is the SERVER PORT!!!
	int rc = UDP_FillSockAddr(&saddr, hostname, portIn);
	assert(rc == 0);


	msg message;
	retval = 0;
	sprintf(message.buffer, "testing requester to sender");
	rc = UDP_Write(sd, &saddr,(char *) &message, sizeof(message));
	while(1){
		struct sockaddr_in raddr;
		rc = UDP_Read(sd, &raddr,(char *) &message, sizeof(message)); 
		if(rc>0)
			printf("message.buffer: %s\n", message.buffer);
	}
	return 1;
}
*/

