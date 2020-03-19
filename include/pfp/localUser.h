#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <iostream>

#include "algorithm/algorithm.h"
#include "pfp/pfp.h"
#include "pfp/remoteNode.h"

namespace pfp
{

class localUser
{
	public:
		localUser() {};
		~localUser() {};

		localUser(std::string filename); // allows connecting to network where nodes are in file

		void main();

	protected:
		int 	connectToNode(pfp::remoteNode node); // sets up socket connection
		void	workManager(); // creates work orders, removes aswers
		void	handleConnection(int socketFD); // sends work order, waits for reply, adds ans and free node
		void 	workCoordinator(); // removes work orders and sends to free nodes
		
	private:
		std::queue<pfp::WorkOrder>	jobs_; // to send to nodes
		std::queue<pfp::WorkResponse>	answers_;
		alg::INT			n_; // number to factor
		std::vector<std::thread>	threadPool_;
		std::vector<alg::INT>		factors_;
		bool				stillWorking_;
		std::queue<pfp::remoteNode>	freeNodes_; // holds the list of nodes read in from file 
};





}
