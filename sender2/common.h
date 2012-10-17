#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>

#include <unistd.h>

#define MAXLINESIZE 256
#define MAXPAYLOAD 1024
#define DEBUG 0
/*
typedef struct msg_header_t{
	char type;
	uint sequence;
	uint length;
}msg_header;
*/
typedef struct msg_t{
	char type;
	uint sequence;
	uint length;
char payload[MAXPAYLOAD];
}msg;
void errorExit(char * errorMessage){
	fprintf(stderr, "Error: %s\n", errorMessage);
	exit(1);
}

void printIP(struct sockaddr_in * saddr){
	printf("IP: %d.%d.%d.%d\n",
  (int)(saddr->sin_addr.s_addr&0xFF),
  (int)((saddr->sin_addr.s_addr&0xFF00)>>8),
  (int)((saddr->sin_addr.s_addr&0xFF0000)>>16),
  (int)((saddr->sin_addr.s_addr&0xFF000000)>>24));
}
void printMessage(msg * message){
	printf("message.type:%c\n message.sequence:%d\n message.length:%d\n message.payload:%s\n",message->type, message->sequence, message->length, message->payload);
} 
