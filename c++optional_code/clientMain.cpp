#include "Client.h"
#include <memory>
#include <vector>
#include <thread>
#include <future>


int doWork(std::unique_ptr<Client> c){
    return c->connected();
}


int main()
{
    std::string userInput;
    std::cout << "Put in number to be factorized -> ";
    getline(std::cin, userInput);

    //HANDLE USER INPUT HERE IF THE SERVERS NEED TO BE SENT SOMETHING DIFFERENT

    /*
    std::vector<std::string> servers = {"45.76.232.110", "144.202.74.41"};
    int port = 10000;
    for (const auto& s : servers) {
        std::unique_ptr<Client> c = std::make_unique<Client>(s, port);
        c->createSocket();
        c->setWork(std::stoi(userInput));
        std::future<int> fut = std::async(doWork, std::move(c));
        int response = fut.get();
        std::cout << response;
        c->shutdown();
    }*/

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
    }

	return 0;

}

