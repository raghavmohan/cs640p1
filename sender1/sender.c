#include "udp.h"
#include "sender.h"

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

//	int sd = init(getHost());
	int sd = UDP_Open(senderPort);
	assert(sd > -1);

	printf("waiting in loop\n");
	while (1) {
		//use this to get the socket address
		//struct sockaddr_in y;
		//saddr= y;
		char buffer[4096];
		int rc = UDP_Read(sd, &saddr, buffer, 4096);

		
			printf("buffer: %s\n", buffer);
		//wait for a request header
		if (rc > 0) {
		//	printf("buffer: %s\n", buffer);

			//handleMessage (buffer);
			//need a message struct casted as a char []
			rc =  UDP_Write(sd, &saddr, buffer, sizeof(packet));
		}
	}



	



	char* inFile = "./split.txt";


	int fd = open(inFile, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Error: Cannot open file %s\n", inFile);   
		exit(1);
	}
	struct stat file_status;
	if(stat(inFile, &file_status) != 0){
		fprintf(stderr, "Error: Cannot stat file %s\n", inFile);
		exit(1);
	}
	int fileSize = (int) file_status.st_size;

	int numPackets = (int)ceil( (double)fileSize/ (double)length);
	packet * packets = malloc ( numPackets * sizeof(packet));

	if(DEBUG){
		printf("numPackets: %d\n", numPackets);
		printf("filesize: %d\n", fileSize);
	}

	int counter =0;
	char r ='\0';
	//int rc = UDP_Read(sd, &
	while (1) {	
		int rc;
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




	return 0;
}
