#include "common.h"
#include <math.h>

#define BUFFER_SIZE 4096
#define DEBUG 1

//TODO: Check the length of the types...
typedef struct packet_t{
	char type;
	uint sequence;
	size_t length;
	char * payload;
} packet;

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

void errorExit(char * errorMessage){
	fprintf(stderr, "Error: %s\n", errorMessage);
	exit(1);
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


int DFS_Write(packet * packetToWrite){
	int rc;
	int retval = 0;

	int sd = -1;

	while(retval == 0){
		rc = UDP_Write(sd, &saddr,(char *) packetToWrite, sizeof(packet));
		if (rc > 0){
			struct sockaddr_in raddr;
			rc = UDP_Read(sd, &raddr,(char *) packetToWrite, sizeof(packet)); 
		}
	}

	return 0;
	//return message.returnVal;
}
