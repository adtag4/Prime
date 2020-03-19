#include <sstream>
#include <thread>
#include <algorithm/pollard.h>
#include <algorithm/ecm.h>
#include <algorithm/quadratic.h>
#include "pfp/localNode.h"
#include <stdio.h>

namespace pfp
{

// Server constructor with port to run on
localNode::localNode(int port) : listenPort_(port)
{
    	// create the listening thread which will run the whole life
	shutdown_ = false;
	cycles_ = 1000;
}

void localNode::main()
{
	std::thread lThread(&localNode::setupListener, this); 
    	std::cout << "Created Listener Socket" << std::endl;

	std::thread workThread(&localNode::work, this); // start work thread
	
	std::thread corona(&localNode::corona, this); // kills you in 14 days
	
	// this is blocking so will not continue until lThread is done
    	lThread.join();	
	workThread.join();
}

void localNode::corona()
{
	std::this_thread::sleep_for(std::chrono::hours(1)); // when symptoms show
	shutdown_ = true;
}

void localNode::work()
{
	while(!shutdown)
	{
		while(jobs_.empty()); // wait until there are jobs to do

		std::cout << "localNode::work found a job to do" << std::endl;		

		pfp::WorkOrder currentJob = jobs_.front();
		jobs_.pop();
		
		std::cout << "localNode::work working on: " << currentJob << std::endl;

		std::stringstream ss; // source string
		std::stringstream as; // answer string
		ss.str(currentJob.getEncodedState());
		switch(currentJob.getAlgorithm())
		{
			case pfp::ALG::PR:
			{
				std::cout << "localNode::work Job is pollard" << std::endl;
				alg::PollardState ps;
				ss >> ps;
				alg::Pollard p(ps);
				alg::runXTimes(as, cycles_, p);
				break;
			}
			case pfp::ALG::ECM:
			{
				std::cout << "localNode::work Job is ECM" << std::endl;
				alg::ECMState es;
				ss >> es;
				alg::ECM ecm(es);
				alg::runXTimes(as, cycles_, ecm);
				break;
			}
			case pfp::ALG::QS:
			{
				std::cout << "locclNode::work Job is QS" << std::endl;
				alg::QuadSieveState qss;
				ss >> qss;
				alg::QuadSieve qs(qss);
				alg::runXTimes(as, 1, qs); // different because only runs once.  
				break;
			}
			default:
				std::cerr << "Wierd input lol" << std::endl; // none algorithm - just ignore
		}
		pfp::WorkResponse wr(currentJob.getAlgorithm(), currentJob.getEncodedState(), as.str());
		
		
		std::unique_lock<std::mutex> lock(answer_mutex_);
		answers_.push_back(wr); // add to list of finished work

		// and RAII cleans up!
	}
}

int localNode::setupListener()
{
	// Create a socket
	listenSocket_ = socket(AF_INET, SOCK_STREAM, 0);
	if(listenSocket_ == -1)
	{
		std::cerr << "Failed to create socket"; 
		return -1;
	}
	// bind the socket to an IP

	hint_.sin_family = AF_INET;
	hint_.sin_port = htons(listenPort_);
	inet_pton(AF_INET, "0.0.0.0", &hint_.sin_addr); // 0.0.0.0 puts on any IP
	if(bind(listenSocket_, (sockaddr*) &hint_, sizeof(hint_)) == -1)
	{
		std::cerr << "Can't bind to IP/Port";
		return -1;
	}

	// Mark the socket for listening
	if(listen(listenSocket_, SOMAXCONN) == -1) // SOMAXCONN allows for max connections 
	{
		std::cerr << "Server can't listen";
		return -1;
	}
	while(!shutdown_)
	{
		handleClient();
	}
	std::cout << "Closing listener" << std::endl;
	// close listening socket 
	close(listenSocket_);
}

void localNode::handleClient()
{
	// Accept a call
	struct sockaddr_in client; 
	int clientSocket;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];

	std::cout << "Waiting for Connection" << std::endl;
	clientSocket = accept(listenSocket_, (sockaddr*) &client, &clientSize);
	if(clientSocket == -1)
	{
		std::cerr << "Problem with client connecting";
		return;
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
        std::thread(&localNode::handleUser, this, clientSocket, client).detach();

}

bool localNode::searchForMatch(pfp::WorkOrder& wo, pfp::WorkResponse& wr)
{
	// searches queue/vector of answers for one matching wo given.  
		// if not found - false
		// if found: copy to wr and ret true

	std::unique_lock<std::mutex> lock(answer_mutex_);
	for(auto x = answers_.begin(); x != answers_.end(); x++)
	{
		if(x->getEncodedStart() == wo.getEncodedState())
		{
			wr = *x; // copy out
			answers_.erase(x);
			return true;
		}
	}
	return false;

	// RAII cleans up lock
}

void localNode::handleUser(int userSocket, struct sockaddr_in userAddr)
{
	// while receiving - display message, echo message
	char buf[4096];
	
	// clear the buffer
	memset(buf, 0, 4096);
	// wait for message
	int bytesRecv = recv(userSocket, buf, 4096, 0);
	if(bytesRecv == -1)
	{
		std::cerr << "There was a connection issue" << std::endl;
	}
	if(bytesRecv == 0)
	{
		std::cout << "The client sent a disconnected message" << std::endl;
	}
	//std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
	printf("localNode::handleUser Received: %s\n: ", buf);

	// Use recvd to put a WorkOrder in the queue
	pfp::WorkOrder wo;
	std::stringstream s;
	s.str(buf);
	s >> wo; // decode WorkOrder from input
	std::cout << "localNode::handleUser Work Order: " << wo << std::endl;
	jobs_.push(wo); // add to queue
	
	
	pfp::WorkResponse answer;
	while(!searchForMatch(wo, answer))
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::cout << "localNode::handleUser No answer yet" << std::endl;
	}
	std::cout << "localNode::handleUser found answer: " << answer << std::endl;
	
	// encode and send answer
	std::stringstream ss;
	ss << answer;
	send(userSocket, ss.str().c_str(), sizeof(ss), 0);

	// close client/user socket 
	close(userSocket);
	// thread frees resources when it exits the function
}

}
