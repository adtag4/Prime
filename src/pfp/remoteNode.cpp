#include "pfp/remoteNode.h"

#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

namespace pfp
{

// IP can be a dns name
remoteNode::remoteNode(std::string IP, int port)
	: IP_(IP),
	  port_(port)
{
	// use this ctor
}

int remoteNode::makeConnectionTo()
{
	// Code was taken partly from a project writing malware I did within 2 yrs ago
	int err;

	struct addrinfo		*ai_head;
	struct sockaddr_in	*addr_in;
	struct sockaddr_in	serv_addr;


	// error check
	if(IP_ == "" || port_ == 0)
	{
		std::cerr << "ERROR: attempted to connect to remoteNode without an IP or port" << std::endl;
		exit(0);
	}

	// Perforn DNS host resolution
	err = getaddrinfo(IP_.c_str(), NULL, NULL, &ai_head);
	if(err)
	{
		std::cerr << "Host resolution failed." << std::endl;
		exit(-1);
	}

	addr_in = (struct sockaddr_in *) ai_head->ai_addr; // get result of resolution

	// setup sockaddr for destination
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= addr_in->sin_addr.s_addr;
	serv_addr.sin_port		= htons(port_);


	int sockFD = socket(AF_INET, SOCK_STREAM, 0); // Make TCP/IPV4 socket
	if(sockFD == -1)
	{
		std::cerr << "You suck" << std::endl;
		exit(-1);
	}

	err = connect(sockFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if(err < 0)
	{
		std::cerr << "Failed to connect" << std::endl;
		return -1;
	}

	return sockFD;

	
}

void remoteNode::decode(std::istream& in)
{
	in >> IP_;
	in >> port_;
}

std::istream& operator >> (std::istream& in, remoteNode& data)
{
	data.decode(in);
	return in;
}


}
