#include "pfp/localUser.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include <time.h>
#include <sstream>

namespace pfp
{

// Server constructor with port to run on
localUser::localUser(std::string filename) : 
		stillWorking_(true),
		rc_(gmp_randinit_default),
		numRandomOrders_(10)
{
	// read in entries in file and store them in freeNodes_	
	// create the file if it does not exist 
   	int check = open(filename.c_str(), O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
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

	rc_.seed(time(NULL));		
	numRelsQS_ = 300;
}

// the method that the main code will run
void localUser::main()
{
	std::string userNum; 
	// get the number from the user 
	std::cout << "Please enter your number to be factored. " << std::endl << ">"; 
	std::getline(std::cin, userNum); 
	// initialize n_ and set it
	n_.set_str(userNum, 10);
	std::cout << "Your number is: " << n_ <<  std::endl << "Beginning Solving Process now." << std::endl;
	// create Coordinator and Manager threads 
	std::thread manThread(&localUser::workManager, this); 
	std::thread coordThread(&localUser::workCoordinator, this); 
	// start timing 
	auto start = std::chrono::high_resolution_clock::now();
	

	manThread.join();
	coordThread.join();
	// end timing
	auto end = std::chrono::high_resolution_clock::now(); 
	double program_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

	program_time = program_time/1000000000;

	printSolution();
	std::cout.precision(9);
	std::cout << "PFP took " << std::fixed << program_time << " seconds to find factors." << std::endl;
}

// sets up socket connection
int localUser::connectToNode(pfp::remoteNode node)
{
	//std::cerr << "localUser::connectToNode(" << node.IP_ << ", " << node.port_ << ")" << std::endl;
	return node.makeConnectionTo();
}

// creates work orders, removes answers
void localUser::workManager()
{
	std::unique_lock<std::mutex> l(jobs_mutex_);
	// generate a default Pollard Rho to start 
	alg::INT x = 2_mpz;
	alg::INT y = x;
	alg::INT d = 1_mpz;
	alg::PollardState ps(n_, x, y, d);
	std::stringstream ss;
	ss << ps;
	pfp::WorkOrder firstwo(pfp::ALG::PR, ss.str());
	jobs_.push(firstwo);
	//std::cout << "FIRST JOB:" << firstwo << std::endl;
	
	// Initial work order creation
	for(auto x = 0; x < numRandomOrders_; x++)
	{
		pfp::WorkOrder pollardJob = genRandomPollardOrder(n_);
		pfp::WorkOrder ecmJob     = genRandomECMOrder(n_);
		jobs_.push(pollardJob);
		jobs_.push(ecmJob);
		//std::cout << "ADDED JOB:" << pollardJob << std::endl;
		//std::cout << "ADDED JOB:" << ecmJob << std::endl;
	}
	pfp::WorkOrder qsJob = genQSOrder(n_, numRelsQS_);
	jobs_.push(qsJob);
	l.~unique_lock();

	while(stillWorking_)
	{
		// get first answer in queue
		while(answers_.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		std::unique_lock<std::mutex> a(answers_mutex_);
		pfp::WorkResponse wr = answers_.front();
		answers_.pop();
		a.~unique_lock();

		alg::INT factor;
		// test completeness
		if(factorFound(wr, factor)) // found a factor!  Shutdown and complete
		{
			stillWorking_ = false;
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
		std::unique_lock<std::mutex> j(jobs_mutex_);
		jobs_.push(newOrder); // add to queue
		//std::cout << "***ADDED CONTINUATION ORDER***:" << newOrder << std::endl;
		j.~unique_lock();
	}
}

// sends work order, waits for reply, adds ans and free node
void localUser::handleConnection(int socketFD, pfp::remoteNode node, pfp::WorkOrder myJob)
{
	// send work order over connection
	std::stringstream ss; 
	ss << myJob;
	//std::cout << "JOB SENT IS:" << ss.str().c_str() << " with a size of:" << strlen(ss.str().c_str()) + 1 << std::endl;
	int bytesSent = send(socketFD, ss.str().c_str(), strlen(ss.str().c_str()) + 1, 0);
	// std::cout << "USER SENT __" << bytesSent << "__ BYTES" << std::endl;
	
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
	//std::cout << "RECEIVED " << bytesRecv << " BYTES" << std::endl;
	pfp::WorkResponse result;
	std::string s(buf);
	//std::cout << "RECEIVED ___" << bytesRecv << "___ bytes" << std::endl;
	//std::cout << "RECEIVED INFORMATION IN STRING:" << s << std::endl;
	std::stringstream ss2;
	ss2.str(s);
	//std::cout << "SS2 IS:" << ss2.str().c_str() << std::endl;
	ss2 >> result;
	//std::cout << "RESULT IS:" << result << std::endl;

	// close socket after use
	close(socketFD);

	// add answer to answers_
	std::unique_lock<std::mutex> a(answers_mutex_);
	answers_.push(result);
	a.~unique_lock();

	// put node back in freeNodes_
	std::unique_lock<std::mutex> f(freeNodes_mutex_);
	freeNodes_.push(node);
	f.~unique_lock();
	
}

// removes work orders and sends to free nodes
void localUser::workCoordinator()
{
	// give manager time to start 
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	while(stillWorking_)
	{
		// check for any free nodes
		if(!freeNodes_.empty())
		{
			// there is a free node, assign it and start a thread to deal with it
			std::unique_lock<std::mutex> l(freeNodes_mutex_);
			pfp::remoteNode fNode = freeNodes_.front();
			int newConn = connectToNode(fNode);
			freeNodes_.pop(); // remove the first element 
			l.~unique_lock();
			//std::cout << "REMAINING FREE NODES FOR WORK:" << freeNodes_.size() << std::endl;

			// check to make sure work is there to be done
			while(jobs_.empty());  

			// get available jobs
			std::unique_lock<std::mutex> j(jobs_mutex_);
			pfp::WorkOrder myJob = jobs_.front();
			jobs_.pop();
			j.~unique_lock();
			//std::cout << "Remaining jobs in queue:" << jobs_.size() << std::endl; 
			std::thread(&localUser::handleConnection, this, newConn, fNode, myJob).detach();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

		}


	}

}

/*************************************************
 *         HELPER FUNCTIONS BELOW HERE           *
 *************************************************/
alg::INT localUser::genRandom(alg::INT n)
{
	alg::INT x = rc_.get_z_range(n - 1_mpz);
	x = x + alg::INT("1");
	return x;
}

pfp::WorkOrder localUser::genRandomPollardOrder(alg::INT n)
{
	alg::INT x = genRandom(n);
	alg::INT y = x;
	alg::INT d = 1_mpz;
	alg::PollardState ps(n, x, y, d);
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
	ss << qss ;
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
		pfp::WorkOrder res(wr.getAlgorithm(), wr.getEncodedEnd()); 
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		return res;
		
		//return pfp::WorkOrder(wr.getAlgorithm(), wr.getEncodedEnd()); // the end is the beginning
	}
}

bool localUser::factorFound(pfp::WorkResponse wr, alg::INT& factor)
{
	switch(wr.getAlgorithm())
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

bool localUser::factorFoundPollard(std::string data, alg::INT& factor)
{
	alg::PollardState ps;
	std::stringstream ss;
	ss.str(data);
	ss >> ps;
	if(ps.d_ != 1_mpz && ps.d_ != n_)
	{
		factor = ps.d_;
		return true;
	}
	return false;
}

bool localUser::factorFoundECM(std::string data, alg::INT& factor)
{
	alg::ECMState es;
	std::stringstream ss;
	ss.str(data);
	ss >> es;
	if(es.factor_ != alg::INT("-1") && es.factor_ != alg::INT("0"))
	{
		factor = es.factor_;
		return true;
	}
	return false;
}

bool localUser::factorFoundQS(std::string data, alg::INT& factor)
{
	alg::QuadSieveState qss;
	std::stringstream ss;
	ss.str(data);
	ss >> qss;
	if(qss.factor_ != alg::INT("0"))
	{
		factor = qss.factor_;
		return true;
	}
	return false;
}

bool localUser::factorFail(pfp::WorkResponse wr)
{
	switch(wr.getAlgorithm())
	{
		case pfp::ALG::PR:
			return factorFailPollard(wr.getEncodedEnd());
		case pfp::ALG::ECM:
			return factorFailECM(wr.getEncodedEnd());
		case pfp::ALG::QS:
			return factorFailQS(wr.getEncodedEnd());
		default:
			return true;
	}
}

bool localUser::factorFailPollard(std::string data)
{
	alg::PollardState ps;
	std::stringstream ss;
	ss.str(data);
	ss >> ps;
	if(ps.d_ == ps.n_)
	{
		return true;
	}
	if(ps.d_ == alg::INT("0"))
	{
		return true;
	}
	return false;
}

bool localUser::factorFailECM(std::string data)
{
	alg::ECMState es;
	std::stringstream ss;
	ss.str(data);
	ss >> es;
	if(es.factor_ == alg::INT("-1"))
	{
		return true;
	}
	return false;
}

bool localUser::factorFailQS(std::string data)
{
	return false; // just need more 
}

void localUser::shutdown(alg::INT factor)
{
	factors_.push_back(factor);
	factors_.push_back(n_/factor);
}

void localUser::printSolution()
{
	std::cout << "The factors to: " << std::endl << n_ << std::endl << "are: " << std::endl;
	std::cout << "P=" << factors_[0] << std::endl << "Q=" << factors_[1] << std::endl;
}




}
