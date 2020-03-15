CC=g++

CPP_OPTS=-std=c++17
LIBS=-lpthread -lgmp -lgmpxx

all: node user

node: src/node_main.cpp src/pfp/localNode.cpp src/pfp/remoteNode.cpp src/pfp/pfp.cpp src/pfp/remoteUser.cpp
	$(CC) -o $@ $^ $(CPP_OPTS) $(LIBS)

user: 
	echo "user command: TODO"

clean:  
	rm node
