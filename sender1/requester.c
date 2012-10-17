#include "requester.h"
int main (int argc, char ** argv){
	if(argc != 5)
		usage(1);

	getoptions(argc, argv);

	if(DEBUG){
		printf("port: %d\n", port);
		printf("fileOpt: %s\n", fileOpt);
	}

	int fd, bytesRead, fileSize, counter, i;

	char* inFile = "./tracker.txt";


	FILE * trackFp =fopen(inFile, "r");
	if(trackFp == NULL)	
		errorExit("bad Tracker");

	int numlines = numLines(inFile);
	if(DEBUG)
		printf("Numlines:%d\n", numlines);

	tracker * tracks = malloc ( numlines * sizeof(tracker));
	buildTracker(tracks, trackFp);
	//sort the tracks to make easier for later. Look @ compare to see sort order
	qsort (tracks, numlines, sizeof(tracks[0]), compare);
	if(DEBUG){
		for(i =0; i < numlines ; ++i)
			printTrack(tracks[i]);
	}

	//now we have all information in memory
	int numChunks = getNumChunks(tracks, fileOpt, numlines);
	printf("numChunks:%d\n", numChunks);
	sd = UDP_Open(-1);
	assert(sd > -1);

for(i=1; i <= numChunks; ++i){
		packArgs argsP = getIDParams(tracks, fileOpt, i, numlines);
		if(DEBUG > 1)
			printf("host:%s, port:%d\n", argsP.hostname, argsP.port);

		getPackets(argsP.hostname, argsP.port);
	}

	return 0;
}

