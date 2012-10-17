#include "common.h"
#include <math.h>

#define BUFFER_SIZE 4096

//TODO: Check the length of the types...
void getoptions (int argc, char **argv);

//define globals
int senderPort;
int requesterPort;
int rate;
uint length, sequenceNum;
struct sockaddr_in saddr;

void usage(int terminate){
	printf("Usage sender -p <port> -g <requester port> -r <rate> -q <seq_no> -l <length>\n");
	if(terminate)
		exit(0);
}

void getoptions (int argc, char **argv) {
	int c;
	while ((c = getopt (argc, argv, "v?hRs:P:p:G:g:R:r:Q:q:L:l:")) != -1){
		switch (c) {
			case 'h':
				usage(1);
			case 'p':
				senderPort =atoi(optarg);
				if(senderPort < 1024 || senderPort > 65536)
					errorExit("Sender Port must be between 1024 and 65536");
				break;
			case 'g':
				requesterPort = atoi(optarg);
				if(requesterPort < 1024 || requesterPort > 65536)
					errorExit("Request Port must be between 1024 and 65536");
				break;
			case 'r':
				rate = atoi(optarg);
				if(rate < 0){
					errorExit("Rate must be > 0");
				}

				break;
			case 'q':
				//sequenceNum = (uint) atoi(optarg, NULL, 0);
				sequenceNum = (uint) atoi(optarg);
				if(sequenceNum < 0){
					errorExit("sequenceNum must be > 0");
				}

				break;
			case 'l':
				//length = (uint) atoi(optarg, NULL, 0);
				length =  atoi(optarg);
				if(length < 0){
					errorExit("length must be > 0");
				}

				break;
			default:
				break;
		}
	}
} 

int init(char *hostname){
	//-1 lets the OS choose a free port  
	int sd = -1;
	sd = UDP_Open(senderPort);
	assert(sd > -1);

	int rc = -1;
	rc = UDP_FillSockAddr(&saddr, hostname, requesterPort);
	assert(rc == 0);
	return rc;
}

char * getHost(){
	char hostname[1024];
	hostname[1023] = '\0';
	gethostname(hostname, 1023);
	return strdup(hostname);
}

//routine that takes in a request packet
//processes and returns an array of packets to send
msg *  processMessage(msg * message, int * numP){
	int numPackets;
	msg * packets;
	if(message->type == 'R'){
		char * inFile = message->payload;
		printf("infile: %s\n", inFile);
		FILE * dataFile =fopen(inFile, "r");
		if(dataFile == NULL)
			return NULL;	

		struct stat file_status;
		if(stat(inFile, &file_status) != 0){
			fprintf(stderr, "Error: Cannot stat file %s\n", inFile);
			exit(1);
		}
		int fileSize = (int) file_status.st_size;

		numPackets = (int)ceil( (double)fileSize/ (double)length);
		packets = malloc ( numPackets * sizeof(msg));

		if(DEBUG){
			printf("numPackets: %d\n", numPackets);
			printf("filesize: %d\n", fileSize);
		}
		int i, j;
		for(i = 0; i < numPackets; ++i){
			if(i==(numPackets-1))
				packets[i].type='E';
			else
				packets[i].type='D';
			packets[i].length=length;
			//TODO : need to figure this out
			packets[i].sequence=i;
			for(j=0; j < length; ++j){
				char ch = getc(dataFile);
				if(ch != EOF)
					packets[i].payload[j] = ch;
			}	 
		}

		if(DEBUG){
			for(i = 0; i < numPackets; ++i)	
				printMessage(&packets[i]);
		}
	}
	*numP = numPackets;
	return packets;
}

