#include <cstdlib>
#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

void getoptions (int argc, char **argv);

int port;
string* fileOpt;

void getoptions (int argc, char **argv) {
	int c;
	while ((c = getopt (argc, argv, "v?hRs:O:o:P:p:")) != -1){
		switch (c) {
			case 'h':
				cout<<"Usage requester -p <port> -o <fileoption> -r <rate> -q <seq_no> -l <length>";
				break;
			case 'p':
				port =atoi(optarg);
				break;
			case 'o':
				fileOpt = new string();
				//printf("%s", optarg);
				*fileOpt = optarg;
				break;
			default:
				break;
		}
	}
} 

int main (int argc, char ** argv){
	getoptions(argc, argv);
	cout << "port: "<<port<<endl;
	cout << "fileOpt: "<<*fileOpt<<endl;
	return 0;
}

