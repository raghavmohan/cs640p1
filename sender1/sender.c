#include <stdlib.h>
#include <string.h>
#include "udp.h"

#define BUFFER_SIZE 4096
#define DEBUG 1


void getoptions (int argc, char **argv);

//define globals
int senderPort;
int requesterPort;
//maybe int?
int rate;
uint length, sequenceNum;

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
				break;
			case 'g':
				requesterPort = atoi(optarg);
				break;
			case 'r':
				rate = atoi(optarg);
				break;
			case 'q':
				//sequenceNum = (uint) atoi(optarg, NULL, 0);
				sequenceNum = (uint) atoi(optarg);
				break;
			case 'l':
				//length = (uint) atoi(optarg, NULL, 0);
				length =  atoi(optarg);
				break;
			default:
				break;
		}
	}
} 


int main (int argc, char ** argv){
	getoptions(argc, argv);
	if(argc != 11)
		usage(1);
	
	if(DEBUG){
		printf("senderPort: %d\n", senderPort);
		printf("requesterPort: %d\n", requesterPort);
		printf("rate: %d\n", rate);
		printf("sequenceNum: %d\n", senderPort);
		printf("length: %d\n", senderPort);
	}
int sd = UDP_Open(10000);
	assert(sd > -1);
	while (1) {
		struct sockaddr_in s;
		char buffer[BUFFER_SIZE];
		int rc = UDP_Read(sd, &s, buffer, BUFFER_SIZE);
		if (rc > 0) {
			char reply[BUFFER_SIZE];
			sprintf(reply, "reply");
			rc = UDP_Write(sd, &s, reply, BUFFER_SIZE);
		}
	}






	return 0;





}
