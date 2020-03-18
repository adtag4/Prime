#include "pfp/localNode.h"

// Server constructor with port to run on
localNode::localNode(int port) : listenPort_(port)
{
    	// create the listening thread which will run the whole life
    	std::thread lThread(&localNode::setupListener, this); 
    	std::cout << "Created Listener Socket" << std::endl;
	// this is blocking so will not continue until lThread is done
    	lThread.join();	
}

int localNode::setupListener()
{
	// Create a socket
	listenSocket_ = socket(AF_INET, SOCK_STREAM, 0);
	if(listenSocket_ == -1)
	{
		std::cerr << "Failed to create socket"; 
		return -1;
	}
	// bind the socket to an IP

	hint_.sin_family = AF_INET;
	hint_.sin_port = htons(listenPort_);
	inet_pton(AF_INET, "0.0.0.0", &hint_.sin_addr); // 0.0.0.0 puts on any IP
	if(bind(listenSocket_, (sockaddr*) &hint_, sizeof(hint_)) == -1)
	{
		std::cerr << "Can't bind to IP/Port";
		return -1;
	}

	// Mark the socket for listening
	if(listen(listenSocket_, SOMAXCONN) == -1) // SOMAXCONN allows for max connections 
	{
		std::cerr << "Server can't listen";
		return -1;
	}
	while(_shutdown != 1)
	{
		handleClient();
	}
	std::cout << "Closing listener" << std::endl;
	// close listening socket 
	close(listenSocket_);
}

void handleClient()
{
	// Accept a call
	struct sockaddr_in client; 
	int clientSocket;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];

	std::cout << "Waiting for Connection" << std::endl;
	clientSocket = accept(listenSocket_, (sockaddr*) &client, &clientSize);
	if(clientSocket == -1)
	{
		std::cerr << "Problem with client connecting";
		return -1;
	}

	memset(host, 0, NI_MAXHOST);
	memset(svc, 0, NI_MAXSERV);

	int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

	if(result)
	{
		std::cout << host << " connected on " << svc << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
	}
	// create thread for working with client
        threadPool_.emplace_back(std::thread(&localNode::handleUser, this, client, clientSocket));

}

void handleUser(int userSocket, struct sockaddr_in userAddr)
{
	// while receiving - display message, echo message
	char buf[4096];
	while(true)
	{
		// clear the buffer
		memset(buf, 0, 4096);
		// wait for message
		int bytesRecv = recv(userSocket, buf, 4096, 0);
		if(bytesRecv == -1)
		{
			std::cerr << "There was a connection issue" << std::endl;
			break;
		}
		if(bytesRecv == 0)
		{
			std::cout << "The client sent a disconnected message" << std::endl;
			break;
		}
		std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;

		// echo the message back to the client
		send(userSocket, buf, bytesRecv + 1, 0);
	}
	// close client/user socket 
	close(userSocket);
	// close the thread - done by exiting the function
}
