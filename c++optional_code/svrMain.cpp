#include "Server.h"


int main(int argc, char *argv[])
{
    	if(argc > 1)
	{
		std::cout << "Using passed in arguments" << std::endl;
		// Read in the IP address from the command line
		std::string ip_addr(argv[1]);

    		// Read in the port
    		long portval = strtol(argv[2], NULL, 10);
    		if ((portval < 1) || (portval > 65535)) 
		{
    		  	std::cout << "Invalid port. Value must be between 1 and 65535";
    		  	std::cout << "Format: " << argv[0] << " [<max_range>] [<max_threads>]\n";
      			exit(0);
    		}
    		int port = (int) portval;
		
		// create server
		Server s(ip_addr, port);
	}
	else
	{
		std::cout << "Using default port" << std::endl;
    		Server s;
	}
}
	
    //Server s("155.138.235.233", 10000);

    /*
     * setup for node to node connection???
    std::vector<Server> nodes;
    std::vector<std::string> ips = {"45.76.232.110", "144.202.74.41", "155.138.235.233"};
	Server s("127.0.0.1", 9999);

	for (const auto& server : ips) {
        nodes.emplace_back(Server(server, 10000));
    }
	s.createLSocket();

	for (auto node: nodes){
	    node.createLSocket();
	}
	while(true)
	{
		s.setupCSocket();

        for (auto node: nodes){
            node.setupCSocket();
        }
        for (auto node: nodes){
            node.shutdown();
        }
		s.shutdown();
        return 0;
	}*/
