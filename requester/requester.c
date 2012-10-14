#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udp.h"

//using namespace std;
#define FILE_SZ 256

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
//string* fileOpt;
char* fileOpt;
struct sockaddr_in saddr;
int sd;
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
				fileOpt = malloc(FILE_SZ);
				//printf("%s", optarg);
				sprintf(fileOpt,  optarg);
				//*fileOpt = optarg;
				break;
			default:
				break;
		}
	}
} 

int init(){
	//-1 lets the OS choose a free port  

	char * hostname = getHost();
	printf("Hostname:%s\n ", hostname);
	exit(0);
	sd = UDP_Open(-1);
	assert(sd > -1);

	int rc = UDP_FillSockAddr(&saddr, hostname, port);
	assert(rc == 0);
	return rc;
}

int main (int argc, char ** argv){
	if(argc != 5)
		usage(1);

	getoptions(argc, argv);

	printf("port: %d\n", port);
	printf("fileOpt: %s\n", fileOpt);

	//int port = atoi(argv[1]);


	//sprintf(file, argv[2]);

	sd = UDP_Open(port);
	assert(sd > -1);
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

	return 0;
}

