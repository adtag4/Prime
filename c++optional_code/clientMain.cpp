#include "Client.h"
#include "pollard.h"
#include <memory>
#include <vector>
#include <thread>
#include <future>

struct heartbeat{
    int sockfd;
    bool beating = true;
    bool livingconnection = true;
    //something for time?
};

int doWork(std::unique_ptr<Client> c){
    return c->connected();
}

void sendHeartbeats(heartbeat h){
    char buf [1024];
    while (h.beating && h.livingconnection) {
        send(h.sockfd, "heartbeat", sizeof(" heartbeat"), 0);
        //wait
        if (recv(h.sockfd, buf, sizeof(buf), 0) < 0)
            h.livingconnection = false;
    }
    return;
}


int main()
{
    heartbeat heart;
    std::string userInput;
    std::cout << "Put in number to be factorized -> ";
    getline(std::cin, userInput);

    //HANDLE USER INPUT HERE IF THE SERVERS NEED TO BE SENT SOMETHING DIFFERENT

    alg::PollardState p (std::stoi(userInput));
    char *msg = p.encode();

    std::vector<std::string> servers = {"45.76.232.110", "144.202.74.41"};
    int port = 10000;
    for (const auto& s : servers) {
        std::cout << "Connecting on:" << s << " Port " << port << std::endl;
        std::unique_ptr<Client> c = std::make_unique<Client>(s, port);
        c->createSocket();
        c->setWork(msg);
        int socketToClose = c->getSockNum();
        heart.sockfd = socketToClose;

        std::future<int> fut = std::async(doWork, std::move(c));
        int response = fut.get();
        heart.beating = false;
        std::cout << response;
        close(socketToClose);
    }
/*
    std::vector<int> ports = {9999, 9998};
    std::string server = "127.0.0.1";
    for (const auto& p : ports) {
	    std::cout << "Connecting on:" << server << " Port " << p << std::endl;
        std::unique_ptr<Client> c = std::make_unique<Client>(server, p);
        c->createSocket();
        c->setWork(std::stoi(userInput));
        int socketToClose = c->getSockNum();
        std::future<int> fut = std::async(doWork, std::move(c));
        int response = fut.get();
        std::cout << response;
        close(socketToClose);
    }*/

	return 0;

}

