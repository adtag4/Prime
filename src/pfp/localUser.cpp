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
	n.set_str(userNum, 10);
	// create Coordinator and Manager threads 
	std::thread coordThread(&localUser::workCoordinator, this); 
	std::thread manThread(&localUser::workManager, this); 
	


	coordThread.join();
	manThread.join();


}

// sets up socket connection
int connectToNode(pfp::remoteNode node)
{
	node.makeConnectionTo();
}

// creates work orders, removes aswers
void workManager()
{
	while(stillWorking_)
	{
		// create work orders if needed 

		// remove answers 

		// if answer found, send shutdown signal 

	}
}

// sends work order, waits for reply, adds ans and free node
void handleConnection(int socketFD)
{
	// send work  order over connection

	// wait for answer

	// add answer to answers_

	// put node back in freeNodes_
	
}

// removes work orders and sends to free nodes
void workCoordinator()
{
	while(stillWorking_)
	{
		// check for any free nodes

		// if there is a free node, assign it and start a thread to deal with it

		// connecToNode()


	}

}



}
