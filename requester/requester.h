#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udp.h"
#include <arpa/inet.h>
#include "common.h"


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

void printReceipt(msg * message){
	printf("message.type:%c\n message.sequence:%d\n message.length:%d\n message.payload:%s\n",message->type, message->sequence, message->length, message->payload);
}
//This get from the TRACKER txt
int  getPackets(char * hostname, int portIn){

	//This is the SERVER PORT!!!
	int rc = UDP_FillSockAddr(&saddr, hostname, portIn);
	assert(rc == 0);

	msg message;

	retval = 0;
	message.type ='R';
	message.sequence =0;
	message.length=0;
	memcpy(message.payload, fileOpt, strlen(fileOpt)+1);
	rc = UDP_Write(sd, &saddr,(char *) &message, sizeof(message));
	int sentinel = 0;
	int numPackets = 0, numBytes=0;
	long mtimeTotal=0;
	while(!sentinel){
	
	struct timeval startTotal, endTotal; 
	gettimeofday(&startTotal, NULL);
		struct sockaddr_in raddr;
		rc = UDP_Read(sd, &raddr,(char *) &(message), sizeof(message)); 
		if(rc>0){
			gettimeofday(&endTotal, NULL);
			time_t curtime;
			curtime=endTotal.tv_sec;
			char timeBuf[32];
			strftime(timeBuf,30,"%m-%d-%Y  %T.",localtime(&curtime));
			
			printf("IP:%s\n", inet_ntoa(raddr.sin_addr));
			printf("Time rec:%s\n", timeBuf);

			if(message.type=='E')
				sentinel = 1;
			++numPackets;
			numBytes+= rc;


			long mtime, seconds, useconds;
			seconds  = endTotal.tv_sec  - startTotal.tv_sec;
			useconds = endTotal.tv_usec - startTotal.tv_usec;
			mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
			mtimeTotal +=mtime;

			//printReceipt(&message);
		}


	}
				
		printf("Total Data Packets:%d\n", numPackets);
		printf("Total Bytes:%d\n", numBytes);
		double avgPack = ((double) numPackets)/ ((double) mtimeTotal/1000);
		printf("Avg Packets/sec:%f\n", avgPack);

	return 1;
}


