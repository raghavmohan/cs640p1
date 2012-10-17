#include "udp.h"
#include "sender.h"

int main (int argc, char ** argv){
	int i;
	getoptions(argc, argv);
	if(argc != 11)
		usage(1);

	if(DEBUG){
		printf("senderPort: %d\n", senderPort);
		printf("requesterPort: %d\n", requesterPort);
		printf("rate: %d\n", rate);
		printf("sequenceNum: %d\n", sequenceNum);
		printf("length: %d\n", length);
	}

	//	int sd = init(getHost());
	int sd = UDP_Open(senderPort);
	assert(sd > -1);

	printf("waiting in loop\n");
	int rc = 0; 
	msg message;
	while (rc <=0) {
		//use this to get the socket address
		struct sockaddr_in y;
		saddr= y;
		rc = UDP_Read(sd, &saddr, (char *)&message,sizeof(message) );


		//	printMessage(&message);

	}
	int numPackets=0;
	msg * packetsToSend = processMessage(&message, &numPackets);
	if(DEBUG){
		for(i = 0; i < numPackets; ++i)	
			printMessage(&packetsToSend[i]);
		printf("numPackets: %d\n", numPackets);
	}
	for (i= 0; i < numPackets; ++i){
		rc =  UDP_Write(sd, &saddr,(char*) &(packetsToSend[i]), sizeof(message));
		sleep(1);
	}
	/*
	   message.type='D';
	   message.sequence=sequenceNum;
	   message.length=length;
	   memcpy(message.payload, "testing sender to requester", strlen("testing sender to requester"));
	   rc =  UDP_Write(sd, &saddr,(char*) &message, sizeof(message));
	   printf("rc is %d\n", rc);
	   exit(0);
	   */
	return 0;
}
