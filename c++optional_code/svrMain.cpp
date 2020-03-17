#include "Server.h"


int main()
{
    std::vector<Server> nodes;
    std::vector<std::string> ips = {"45.76.232.110", "144.202.74.41"};
	Server s("127.0.0.1", 9999);
	/*
	for (const auto& server : ips) {
        nodes.emplace_back(Server(server, 10000));
    }*/

	s.createLSocket();
	/*
	for (auto node: nodes){
	    node.createLSocket();
	}*/
	while(true)
	{
		s.setupCSocket();
		/*
        for (auto node: nodes){
            node.setupCSocket();
        }*/
		/*
        for (auto node: nodes){
            node.shutdown();
        }*/
		s.shutdown();
        return 0;
	}
}

