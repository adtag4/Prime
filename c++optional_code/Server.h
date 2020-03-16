#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

class Server
{
public:
	Server();
	Server(std::string serverIP, int port);
	~Server(); 

// functions a Server should be able to do 
	int createLSocket();	
	int setupCSocket();
	int serverWork(sockaddr_in client, int clientSocket);
	int shutdown();

protected:

private:

	std::string _svrIP;
	int _port;
	int _listening;
	sockaddr_in _hint; 
	//sockaddr_in _client; 
	//int _clientSocket;
	std::vector<std::thread> tPool;

	// shutdown signal
	int _shutdown = 0;

};


#endif
