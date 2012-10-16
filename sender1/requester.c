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
typedef struct msg_t{
	char buffer[1024];
}msg;

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

char * getIDHost(tracker * tracks, char * fileOpt,int id, int trackLen){
	int i = 0;
	for(i; i < trackLen; ++i){
		if((strcmp(tracks[i].file, fileOpt) == 0) && (tracks[i].id == id))
			return tracks[i].mac;
	}
	return '\0';
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

	if(DEBUG){
		for(i =0; i < numlines ; ++i)
			printTrack(tracks[i]);
	}

	//now we have all information in memory

	int numChunks = getNumChunks(tracks, fileOpt, numlines);
	printf("numChunks:%d\n", numChunks);
	for(i=1; i <= numChunks; ++i){
		printf("host:%s\n", getIDHost(tracks, fileOpt,i, numlines));
	}



	//exit(0);
	//Get this from the arguments
	//This is the client port aka the requester port
	sd = UDP_Open(-1);
	assert(sd > -1);

	//This get from the TRACKER txt
	char *hostname = "mumble-18.cs.wisc.edu";
	
	//This is the SERVER PORT!!!
	int rc = UDP_FillSockAddr(&saddr, hostname, port);
	assert(rc == 0);


	msg message;
	retval = 0;
	while(retval == 0){
		rc = UDP_Write(sd, &saddr,(char *) &message, sizeof(message));
		FD_ZERO(&rfds);
		FD_SET(sd, &rfds);

		tv.tv_sec = 5;
		tv.tv_usec = 0;
		retval = select(sd + 1, &rfds, NULL, NULL, &tv);

		if(retval){
			if (rc > 0) {
				struct sockaddr_in raddr;
				rc = UDP_Read(sd, &raddr,(char *) &message, sizeof(message)); 
			}
		}
	}
	printf("message.buffer: %s\n", message.buffer);

/*
	char buffer[1024];
	sprintf(buffer, "thisisatest");

	printf("%d\n", htons(saddr.sin_port));
	char ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(saddr.sin_addr), ip_str, INET_ADDRSTRLEN);
	printf("%s\n",ip_str);
	rc =  UDP_Write(sd1, &saddr, buffer, 1024);
	if(rc < 0){
		fprintf(stderr, "Write failed");
		exit(1);
	}
	while(1){
	struct sockaddr_in raddr;
	rc = UDP_Read(sd1, &raddr, buffer, 1024);
	if(rc > 0)
		printf(buffer);
	}
*/
	/*
	   printf("waiting in loop\n");
	   while (1) {
//use this to get the socket address
struct sockaddr_in y;
saddr = y;
char buffer[sizeof(msg_t)];
int rc = UDP_Read(sd, &saddr, buffer, sizeof(msg_t));

//figure out what kind of message this is - read, write, lookup etc
//when done go ahead and reply to it

if (rc > 0) {
// char reply[BUFFER_SIZE];
//sprintf(reply, "reply");

printf("Printing buffer\n");
int i = 0;
for(i; i < sizeof(buffer); ++i)
printf("%c", buffer[i]);
printf("\n");
//	handleMessage (buffer);
//need a message struct casted as a char []
rc =  UDP_Write(sd, &saddr, buffer, sizeof(msg_t));
}
}
*/
return 0;
}

