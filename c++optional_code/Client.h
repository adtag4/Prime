#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <vector>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <string>

class Client
{
public:
	Client();
	Client(std::string serverIP, int port);
	~Client(); 

// functions a client should be able to do 
	int createSocket();
	int connected();
	int shutdown();
	void setWork(int);
	int getSockNum();
    std::vector<std::string> parsestr (std::string);
    std::string createMsg (int, int, int, std::string);

protected:

private:
    int _work;
    std::string _delimiter = "#";
   	std::string _svrIP;
	int _port;
	int _sock;
	sockaddr_in _hint;
	int _finished = 0; 
	int _msgSize = 4096;


};


#endif
