#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udp.h"
#include <arpa/inet.h>
//#include <netin/tcp.h>
//#include <netin/in.h>

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
char* fileOpt;
static struct sockaddr_in saddr;
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
				sprintf(fileOpt,  optarg);
				break;
			default:
				break;
		}
	}
} 

int init(){
	//-1 lets the OS choose a free port  

	//	sd = UDP_Open(-1);
	//sd = UDP_Open(-1);
	printf("port:%d\n", port);
	sd = UDP_Open(port);
	assert(sd > -1);

	//printf("%s\n", hostname);
	char *hostname = "mumble-14.cs.wisc.edu";
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

	//Get this from the arguments
	//This is the client port aka the requester port
	int sd1 = UDP_Open(6000);
	assert(sd1 > -1);

	//printf("%s\n", hostname);
	char *hostname = "mumble-14.cs.wisc.edu";
	//This get from the TRACKER txt
	//This is the SERVER PORT!!!
	int	 rc = UDP_FillSockAddr(&saddr, hostname, port);
	assert(rc == 0);

	char buffer[1024];
	sprintf(buffer, "thisisatest");

	printf("%d\n", htons(saddr.sin_port));
	char ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(saddr.sin_addr), ip_str, INET_ADDRSTRLEN);
	printf("%s\n",ip_str);
	rc =  UDP_Write(sd1, &saddr, buffer, 1024);
	perror("helloworld");

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

