CC=g++

CPP_OPTS=-std=c++17
LIBS=-lpthread -lgmp -lgmpxx -lmpfr
INC=-Iinclude

ALG_FILES=src/algorithm/algorithm.cpp src/algorithm/ecm.cpp src/algorithm/pollard.cpp src/algorithm/quadratic.cpp

all: node user

node: src/node_main.cpp src/pfp/localNode.cpp src/pfp/remoteNode.cpp src/pfp/pfp.cpp src/pfp/remoteUser.cpp src/algorithm/pollard.cpp src/algorithm/algorithm.cpp src/algorithm/ecm.cpp
	$(CC) -o $@ $^ $(CPP_OPTS) $(LIBS) $(INC)

user: 
	echo "user command: TODO"

clean:  
	rm node

test: test.cpp $(ALG_FILES)
	$(CC) -o $@ $^ $(CPP_OPTS) $(LIBS) $(INC)
