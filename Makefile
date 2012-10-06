# Makefile
# Requires that the object definitions be in wl.h and wl.cpp

CXX =           g++
PURXX = purify
CXXFLAGS =      -O2 -g

all: requester sender

requester: requester.cpp 
	$(CXX) $(CXXFLAGS) requester.cpp -o $@

sender: sender.cpp 
	$(CXX) $(CXXFLAGS) sender.cpp -o $@

clean:
	rm -f *.o *~ sender requester

