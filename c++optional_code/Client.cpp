#include "Client.h"

// If a client is initialized with an IP to connect to
Client::Client(std::string serverIP, int port) : _svrIP(serverIP), _port(port)
{
}

// Constructor for if no IP is connected (assume connect to local)
Client::Client() : _svrIP("127.0.0.1"), _port(54000)
{
}

// Client Desctuctor
Client::~Client()
{
}

/*****************************************************************************
 * createSocket() - creates a socket and connects to the server
 * 
 * 
******************************************************************************/
int Client::createSocket()
{
	// create socket
	_sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if(_sock == -1)
	{
		std::cerr << "Can't create socket" << std::endl;
		return -1;
	}

	// fill in hint structure 
	_hint.sin_family = AF_INET;
	_hint.sin_port = htons(_port);
	inet_pton(AF_INET, _svrIP.c_str(), &_hint.sin_addr);

	// connect to server 
	int connResult = connect(_sock, (sockaddr*)&_hint, sizeof(_hint));
	if(connResult == -1)
	{
		std::cerr << "Can't connect to server, Err #" << std::strerror(errno)  << std::endl;
		return -1;
	}
	return 0;
}

void Client::setWork(int newwork){
    _work = newwork;
}

/*****************************************************************************
 * connected() - Loops through and performs actions until time to disconnect 
 * 
 * 
******************************************************************************/
int Client::connected() {
    // do while loop to send and receive data
    int finished = 0;
    char buf[_msgSize];
    std::string userInput;
    do {
        int sendRes = send(_sock, (char *) _work, sizeof(_work) + 1, 0);

        if (sendRes == -1) {
            std::cout << "Failed to send to server." << std::endl;
            shutdown();
        } else {
            std::cout << "work sent to " << _svrIP;
        }

        // wait for response
        memset(buf, 0, _msgSize); // zero the buffer out
        int bytesReceived = recv(_sock, buf, _msgSize, 0);
        // check and see if the sender sent more than buf space
        if (bytesReceived == _msgSize) {
            // TODO:potentially parse data and see if there is more useful
            // for now just 0 out buffer
            std::cout << "Received too much info, very scary" << std::endl;
            while (bytesReceived > 0) {
                memset(buf, 0, _msgSize); // zero the buffer out again
                bytesReceived = recv(_sock, buf, _msgSize, 0);
            }
        } else {
            // display the response, assuming will be int or something of the like
            std::string response = std::string(buf, bytesReceived);
            std::cout << "Server> " << response << std::endl;
            return std::stoi(response);
        }

    }while (finished != -1);
    return 0;
}


/*****************************************************************************
 * shutdown() - Cleanup and shutdown
 * 
 * 
******************************************************************************/
int Client::shutdown()
{
	// shut down socket
	close(_sock);
	return 0;
}


