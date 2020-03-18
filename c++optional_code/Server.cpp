#include "Server.h"

// If a client is initialized with an IP to connect to
Server::Server(std::string serverIP, int port) : _svrIP(serverIP), _port(port)
{
    // create a thread that does the listening
    std::thread lThread(&Server::createLSocket, this); 
    std::cout << "Created Listener Socket" << std::endl;
    lThread.join();

}

// Constructor for if no IP is connected (assume connect to local)
Server::Server() : _svrIP("127.0.0.1"), _port(10000)
{
    // create a thread that does the listening
    //tPool.emplace_back(std::thread([this]{createLSocket();}));
    std::thread lThread(&Server::createLSocket, this); 
    std::cout << "Created Listener Socket" << std::endl;
    lThread.join(); // this pauses the main thread and joins once lThread does it's job
}

// Client Desctuctor
Server::~Server()
{
    shutdown();
}

std::vector<std::string> Server::parsestr (std::string s) {
    size_t pos_start = 0, pos_end, delim_len = _delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (_delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (token != "")
            res.push_back (token);
    }

    //res.push_back (s.substr (pos_start));
    return res;
}


std::string Server::createMsg (int alg, int argc, int arglen, std::string algdata){
    std::string totalmsg;
    totalmsg.append(reinterpret_cast<const char *>(alg));
    totalmsg.append(_delimiter);
    totalmsg.append(reinterpret_cast<const char *>(argc));
    totalmsg.append(_delimiter);
    totalmsg.append(reinterpret_cast<const char *>(arglen));
    totalmsg.append(_delimiter);
    totalmsg.append(algdata);
    return  totalmsg;
}


int Server::createLSocket()
{
    std::cout << "START CREATELSOCKET" << std::endl;
    // Create a socket
    _listening = socket(AF_INET, SOCK_STREAM, 0);
    if(_listening == -1)
    {
        std::cerr << "Failed to create socket";
        return -1;
    }
    // bind the socket to an IP

    _hint.sin_family = AF_INET;
    _hint.sin_port = htons(_port);
    inet_pton(AF_INET, "0.0.0.0", &_hint.sin_addr); // 0.0.0.0 puts on any IP
    if(bind(_listening, (sockaddr*) &_hint, sizeof(_hint)) == -1)
    {
        std::cerr << "Can't bind to IP/Port";
        return -1;
    }

    // Mark the socket for listening
    if(listen(_listening, SOMAXCONN) == -1) // SOMAXCONN allows for max connections
    {
        std::cerr << "Server can't listen";
        return -1;
    }
    std::cout << "PRINT BEFORE LOOP DEBUG" << std::endl;
    while(_shutdown != 1)
    {
        setupCSocket();
    }
    std::cout << "Closing listener" << std::endl;
    // close listening socket
    close(_listening);
}

int Server::setupCSocket()
{
    // Accept a call
    sockaddr_in client;
    int clientSocket;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    std::cout << "Waiting for Connection" << std::endl;
    clientSocket = accept(_listening, (sockaddr*) &client, &clientSize);
    std::cout<< "Listenening port: " << _listening;
    std::cout<< "client port: " << clientSocket;
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
    tPool.emplace_back(std::thread([this, client, clientSocket]{serverWork(client, clientSocket);}));

}


int Server::serverWork(sockaddr_in client, int clientSocket)
{
    // while receiving - display message, echo message
    char buf[4096];
    while(true)
    {
        // clear the buffer
        memset(buf, 0, 4096);
        // wait for message
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
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
        send(clientSocket, buf, bytesRecv + 1, 0);
    }
    // close client socket
    close(clientSocket);
    // Internet says that it closes thread by itself here 


}


/************************************************************************************************
 * shutdown - cleanup for sockets 
 * 
 * 
 ************************************************************************************************/
int Server::shutdown()
{
    // close the client socket
    // close(_clientSocket);
    // clean up all the remaining threads
    int threadNum = tPool.size();
    for(int i = 0; i < threadNum-1; i++)
    {
        std::cout << "Deleting a thread" << std::endl;
        tPool.at(i).join();
    }

}
