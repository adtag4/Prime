#include "pfp/localUser.h"

namespace pfp
{

// Server constructor with port to run on
localUser::localUser(std::string filename) : 
		stillWorking_(true);
{
	// read in entries in file and store them in freeNodes_	
	// create the file if it does not exist 
   	int check = open(filename, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        close(check);
	// open an ifstream to read in
	std::ifstream wFile;
	wFile.open(filename);
	std::string svrName;
	if(wFile.is_open())
	{
		// read in line by line and compare
		while(std::getline(wFile, svrName))
		{
			std::stringstream ss;
			ss.str(svrName);
			remoteNode n;
			ss >> n;
			freeNodes_.push(n);	
		}
		wFile.close();
   	}
}

// the method that the main code will run
void main()
{
	std::string userNum; 
	// get the number from the user 
	std::cout << "Please enter your number to be factored. " << std::endl << ">"; 
	std::getline(std::cin, userNum); 
	// initialize n_ and set it
	mpz_init(n_);
	mpz_set_str(n_, userNum, 10);
	// create Coordinator and Manager threads 
	std::thread coordThread(&localNode::setupListener, this); 


}

// sets up socket connection
int connectToNode(pfp::remoteNode node)
{

}

// creates work orders, removes aswers
void workManager()
{

}

// sends work order, waits for reply, adds ans and free node
void handleConnection(int socketFD)
{

}

// removes work orders and sends to free nodes
void workCoordinator()
{

}



}
