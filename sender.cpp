#include <cstdlib>
#include <iostream>
#include <string>
/*
#include <time>
#include <sys/time>
#include <string>
#include <getopt.h>
*/
using namespace std;

void getoptions (int argc, char **argv);

//define globals
int senderPort;
int requesterPort;
//maybe int?
int rate;
uint length, sequenceNum;


void getoptions (int argc, char **argv) {
	int c;
	while ((c = getopt (argc, argv, "v?hRs:P:p:G:g:R:r:Q:q:L:l:")) != -1){
		switch (c) {
			case 'h':
				cout<<"Usage sender -p <port> -g <requester port> -r <rate> -q <seq_no> -l <length>";
				break;
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
	cout << "senderPort: "<<senderPort<<endl;
	cout << "requesterPort: "<<requesterPort<<endl;
	cout << "rate: "<<rate<<endl;
	cout << "sequenceNum: "<<sequenceNum<<endl;
	cout << "length: "<<length<<endl;
	return 0;
}
