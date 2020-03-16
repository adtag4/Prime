CC=g++

CPP_OPTS=-std=c++17
LIBS=-lpthread #-lgmp -lgmpxx


all: cmain svrmain

cmain: clientMain.cpp	Client.o
	$(CC) $(CPP_OPTS) clientMain.cpp Client.cpp -o cmain $(LIBS)
svrmain: svrMain.cpp Server.o
	$(CC) $(CPP_OPTS) svrMain.cpp Server.cpp -o svrmain $(LIBS)

Server.o: Server.cpp Server.h
	$(CC) -c $(CPP_OPTS) Server.cpp $(LIBS)

Client.o: Client.cpp Client.h
	$(CC) -c $(CPP_OPTS) Client.cpp $(LIBS)


clean: 
	rm *.o cmain svrmain
