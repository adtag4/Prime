#include "pfp/localUser.h"
#include <fstream>
#include <time.h>
namespace pfp
{

// Server constructor with port to run on
localUser::localUser(std::string filename) : 
		stillWorking_(true)
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
void localUser::main()
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

	printSolution();
}

// sets up socket connection
int localUser::connectToNode(pfp::remoteNode node)
{
	return node.makeConnectionTo();
}

// creates work orders, removes answers
void localUser::workManager()
{
	// Initial work order creation
	for(auto x = 0; x < numRandomOrders_; x++)
	{
		pfp::WorkOrder pollardJob = genRandomPollardOrder(n_);
		pfp::WorkOrder ecmJob     = genRandomECMOrder(n_);
		jobs_.push(pollardJob);
		jobs_.push(ecmJob);
	}
	pfp::WorkOrder qsJob = genQSOrder(alg::INT n, numRelsQS_);
	jobs_.push(qsJob);

	while(stillWorking_)
	{
		// get first answer in queue
		while(answers_.empty());
		pfp::WorkResponse wr = answers_.front();
		answers_.pop();

		alg::INT factor;
		// test completeness
		if(factorFound(wr, factor)) // found a factor!  Shutdown and complete
		{
			stillRunning_ = false;
			shutdown(factor);
		}
		// determine course of action
		if(factorFail(wr))
		{
			// Nothing to do, let RAII clean up
			continue;
		}

		// if not fail, but not complete, continue the process
		pfp::WorkOrder newOrder = continueOrder(wr);
		jobs_.push(newOrder); // add to queue
	}
}

// sends work order, waits for reply, adds ans and free node
void localUser::handleConnection(int socketFD, pfp::remoteNode node)
{
	// check to make sure work is there to be done
	while(jobs_.empty());  

	// get available jobs
	pfp::WorkOrder myJob = jobs.front();
	jobs.pop(); 
	
	// send work order over connection
	std::stringstream ss; 
	ss << myJob;
	send(socketFD, ss.str().c_str(), sizeof(ss.str().c_str() + 1), 0);
	
	// wait for answer
	char buf[4096];
	// clear the buffer
	memset(buf, 0, 4096);
	// wait for message
	int bytesRecv = recv(socketFD, buf, 4096, 0);
	if(bytesRecv == -1)
	{
		std::cerr << "There was a connection issue" << std::endl;
	}
	if(bytesRecv == 0)
	{
		std::cout << "The node sent a disconnected message" << std::endl;
	}
	// put information received in work response
	pfp::WorkResponse result;
	std::string s(buf);
	std::stringstream ss;
	ss.str(s);
	ss >> result;

	// add answer to answers_
	answers_.push(result);

	// put node back in freeNodes_
	freeNodes_.push(node);
	
}

// removes work orders and sends to free nodes
void localUser::workCoordinator()
{
	while(stillWorking_)
	{
		// check for any free nodes
		if(!freeNodes_.empty())
		{
			// there is a free node, assign it and start a thread to deal with it
			pfp::remoteNode fNode = freeNodes_.front()
			int newConn = connectToNode(fNode);
			freeNodes_.pop(); // remove the first element 
			
			std::thread(&localUser::handleConnection, this, newConn, fNode).detach();
		}


	}

}

/*************************************************
 *         HELPER FUNCTIONS BELOW HERE           *
 *************************************************/
alg::INT localUser::genRandom(alg::INT n)
{
	gmp_randclass rc(gmp_randinit_default);
	rc.seed(time(NULL));
	auto x = rc.get_z_range(n - 1_mpz);
	x = x + 1_mpz;
	return x;
}

pfp::WorkOrder localUser::genRandomPollardOrder(alg::INT n)
{
	alg::INT x = genRandom(n);
	alg::INT y = genRandom(n);
	alg::INT d = 1_mpz;
	alg::PollardState ps(x, y, d, n);
	std::stringstream ss;
	ss << ps;
	pfp::WorkOrder wo(pfp::ALG::PR, ss.str());
	return wo;
}

pfp::WorkOrder localUser::genRandomECMOrder(alg::INT n)
{
	alg::INT x1 = genRandom(n);
	alg::INT y1 = genRandom(n);
	alg::INT b  = genRandom(n);
	alg::PrePoint pp(x1, y1);
	alg::EllipticCurve ec(pp, b, n);
	alg::Point p(x1, y1);
	alg::ECMState es(p, p, ec);
	std::stringstream ss;
	ss << es;
	pfp::WorkOrder wo(pfp::ALG::ECM, ss.str());
	return wo;
}

pfp::WorkOrder localUser::genQSOrder(alg::INT n, int numRels)
{
	alg::QuadSieveState qss(n, numRels);
	std::stringstream ss;
	ss << qss;
	pfp::WorkOrder wo(pfp::ALG::QS, ss.str());
	return wo;
}

pfp::WorkOrder localUser::continueOrder(pfp::WorkResponse wr)
{
	if(wr.getAlgorithm() == pfp::ALG::QS)
	{
		// just need to increase number of linear relations!
		std::string s = wr.getEncodedEnd();
		std::stringstream ss;
		ss.str(s);
		alg::QuadSieveState qss;
		ss >> qss;
		qss.nbLinRels_ = qss.nbLinRels_ + 10;
		std::stringstream ss2;
		ss2 << qss;
		pfp::WorkOrder wo(pfp::ALG::QS, ss2.str());
		return wo;
	}
	else
	{
		return pfp::WorkOrder(wr.getAlgorithm(), wr.getEncodedEnd()); // the end is the beginning
	}
}

bool localUser::factorFound(pfp::WorkResponse wr, alg::INT& factor)
{
	switch(wr.getAlgorithm)
	{
		case pfp::ALG::PR:
			return factorFoundPollard(wr.getEncodedEnd(), factor);
		case pfp::ALG::ECM:
			return factorFoundECM(wr.getEncodedEnd(), factor);
		case pfp::ALG::QS:
			return factorFoundQS(wr.getEncodedEnd(), factor);
		default:
			return false;
	}
}

bool localUser::factorFoundPollard(std:string data, alg::INT& factor)
{

}

bool localUser::factorFoundECM(std:string data, alg::INT& factor)
{

}

bool localUser::factorFoundQS(std:string data, alg::INT& factor)
{

}






}
