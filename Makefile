CC=g++

CPP_OPTS=-std=c++17
LIBS=-lpthread -lgmp -lgmpxx -lmpfr -lomp
INC=-Iinclude

ALG_FILES=src/algorithm/algorithm.cpp src/algorithm/ecm.cpp src/algorithm/pollard.cpp src/algorithm/quadratic.cpp
QS_FILES=src/QS/QS.cpp src/QS/QS-parallel.cpp src/QS/eratosthenes.cpp src/QS/gmp-patch.c src/QS/math-utils.C src/QS/utils.cpp src/QS/smooth-base.cpp src/QS/smooth-number.cpp src/QS/linear-algebra/matrix.cpp src/QS/linear-algebra/gauss-elimination.cpp

all: node user

node: src/node_main.cpp src/pfp/localNode.cpp src/pfp/remoteNode.cpp src/pfp/pfp.cpp src/pfp/remoteUser.cpp src/algorithm/pollard.cpp src/algorithm/algorithm.cpp src/algorithm/ecm.cpp
	$(CC) -o $@ $^ $(CPP_OPTS) $(LIBS) $(INC)

user: 
	echo "user command: TODO"

clean:  
	rm node

test: test.cpp $(ALG_FILES) $(QS_FILES)
	$(CC) -o $@ $^ $(CPP_OPTS) $(LIBS) $(INC)
