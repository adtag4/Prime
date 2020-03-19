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
		void	handleConnection(int socketFD, pfp::remoteNode node); // sends work order, waits for reply, adds ans and free node
		void 	workCoordinator(); // removes work orders and sends to free nodes
		
	private:
		std::queue<pfp::WorkOrder>	jobs_; // to send to nodes
		std::queue<pfp::WorkResponse>	answers_;
		alg::INT			n_; // number to factor
		std::vector<std::thread>	threadPool_;
		bool				stillWorking_;
		std::queue<pfp::remoteNode>	freeNodes_; // holds the list of nodes read in from file 
		int				numRandomOrders_; // number of random Pollard and ECM orders to start
		int				numRelsQS_;	// number of linear relations to start with for QS
		std::vector<alg::INT>		factors_; // list of found factors to modulus

		// helper functions
		alg::INT	genRandom(alg::INT n); // random between 1...n-1
		pfp::WorkOrder	genRandomPollardOrder(alg::INT n);
		pfp::WorkOrder	genRandomECMOrder(alg::INT n);
		pfp::WorkOrder	genQSOrder(alg::INT n, int numRels);
		pfp::WorkOrder	continueOrder(pfp::WorkResponse wr);
		bool		factorFound(pfp::WorkResponse wr, alg::INT& factor);
		bool		factorFoundPollard(std::string data, alg::INT& factor);
		bool		factorFoundECM(std::string data, alg::INT& factor);
		bool		factorFoundQS(std::string data, alg::INT& factor);
		bool		factorFail(pfp::WorkResponse wr);
		bool		factorFailPollard(pfp::WorkResponse wr);
		bool		factorFailECM(pfp::WorkResponse wr);
		bool		factorFailQS(pfp::WorkResponse wr);
		void		shutdown(alg::INT factor); // divides out modulus and outputs to user.
		void 		printSolution();
};





}
