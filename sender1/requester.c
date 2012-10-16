#include "requester.h"
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

	int fd, bytesRead, fileSize, rc, counter, i;

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
	for(i=0; i < numChunks; ++i){
		printf("host:%s\n", getIDHost(tracks, fileOpt,i+1, numlines));
	}



	exit(0);
	//Get this from the arguments
	//This is the client port aka the requester port
	int sd1 = UDP_Open(6000);
	assert(sd1 > -1);

	//This get from the TRACKER txt
	char *hostname = "mumble-34.cs.wisc.edu";
	//This is the SERVER PORT!!!
	rc = UDP_FillSockAddr(&saddr, hostname, port);
	assert(rc == 0);

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

