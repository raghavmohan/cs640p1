#include "requester.h"

void printTrack(tracker tracktop){
	printf("track.file: %s\ntrack.id: %d\ntrack.mac: %s\ntrack.port: %d\n",tracktop.file, tracktop.id, tracktop.mac, tracktop.port); 
}
int main (int argc, char ** argv){
	if(argc != 5)
		usage(1);

	getoptions(argc, argv);

	if(DEBUG){
		printf("port: %d\n", port);
		printf("fileOpt: %s\n", fileOpt);
	}

	int fd, bytesRead, fileSize, rc, counter;

	char* inFile = "./tracker.txt";


	FILE * trackFp =fopen(inFile, "r");
	if(trackFp == NULL)	
		errorExit("bad Tracker");

	int numlines = numLines(inFile);
	if(DEBUG)
		printf("Numlines:%d\n", numlines);

	tracker * tracks = malloc ( numlines * sizeof(tracker));
	printf("sizeof(tracker): %d\n", sizeof(tracker));

	char linebuf[MAXLINESIZE];	
	int i =0;
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
	for(i =0; i < numlines ; ++i)
		printTrack(tracks[i]);

	
	exit(0);
	/*
	   FILE * trackFp =fopen(inFile, "r");
	size_t linesiz=0;
	char* linebuf=0;
	ssize_t linelen=0;
	while (1){
		linelen=getline(&linebuf, &linesiz, trackFp);
		if(linelen <=0)
			break;
		//this will insert the new line character, get RID
		printf("Linebuf:|%s|" , linebuf);

	}
	exit(0);
*/

	fd = open(inFile, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Error: Cannot open file %s\n", inFile);   
		exit(1);
	}
	struct stat file_status;
	if(stat(inFile, &file_status) != 0){
		fprintf(stderr, "Error: Cannot stat file %s\n", inFile);
		exit(1);
	}
	fileSize = (int) file_status.st_size;

	int numPackets =0;
	//int numPackets = (int)ceil( (double)fileSize/ (double)length);
	//packet * packets = malloc ( numPackets * sizeof(packet));

	if(DEBUG){
		printf("numPackets: %d\n", numPackets);
		printf("filesize: %d\n", fileSize);
	}

	counter =0;
	char r ='\0';
	//int rc = UDP_Read(sd, &
	while (1) {	
		rc = read(fd, &r, 1);
		if (rc == 0) // 0 indicates EOF
			break;
		if (rc < 0) {
			perror("read");
			exit(1);
		}
		printf("%c", r);
		counter++;
	}


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

