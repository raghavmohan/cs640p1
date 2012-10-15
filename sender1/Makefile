# Makefile
# Requires that the object definitions be in wl.h and wl.cpp

CXX =           gcc
PURXX = 	purify
CXXFLAGS =      -O2 -g -lm

all: requester sender

requester: udp requester.o
	$(CXX) $(CXXFLAGS) requester.c -o requester udp.o

sender: udp sender.c 
	$(CXX) $(CXXFLAGS) sender.c -o sender udp.o

udp: udp.c
	$(CXX) -c udp.c -o udp.o
clean:
	rm -f *.o *~ sender requester

