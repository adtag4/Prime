#include "algorithm/algorithm.h"
#include "algorithm/pollard.h"
#include "algorithm/ecm.h"
#include "algorithm/quadratic.h"

#include <iostream>
#include <string>
#include <sstream>

void printRun(alg::Algorithm& a)
{
	for(auto x = 0; x < 100; x++) // run 10 times, printing results each time to std::cout (encoded)
	{
		alg::runXTimes(std::cout, 1, a);
	}
	std::cout << std::endl; // to make output nice :)
}

void test_Pollard_start()
{
	alg::PollardState ps(2262898529_mpz);
	alg::Pollard pollard(ps);
	printRun(pollard);
}

void test_Pollard_decode()
{
	alg::PollardState ps;
	std::stringstream ss;
	ss.str("3 17 1 2262898529");
	ss >> ps;
	alg::Pollard pollard(ps);
	printRun(pollard);
}

void test_ECM_start()
{
	// necessary setup to find curve
	alg::PrePoint pp(2019_mpz, 42_mpz);
	alg::EllipticCurve ec(pp, 7_mpz, 22626898529_mpz);
	alg::Point p(2019_mpz, 42_mpz);
	p.setCurve(&ec);
	alg::ECMState es(p, p, ec);
	alg::ECM ecm(es);
	printRun(ecm);
}

void test_ECM_decode()
{
	alg::ECMState es;
	std::stringstream ss;
	ss.str("2019 42 2019 42 7 4001979 22626898529");
	ss >> es;
	alg::ECM ecm(es);
	printRun(ecm);
}

void test_QS_start()
{
	alg::QuadSieveState qss(22626898529_mpz, 20);
	alg::QuadSieve qs(qss);
	alg::runXTimes(std::cout, 1, qs); // all run once by martani code - not iterative like others
}

void test_QS_decode()
{
	std::stringstream ss;
	ss.str("22626898529 19");
	alg::QuadSieveState qss;
	ss >> qss;
	alg::QuadSieve qs(qss);
	alg::runXTimes(std::cout, 1, qs); // should be same as above, but decoded instead of called
}


int main()
{

	std::cout << "************************************\n" \
		     "*          POLLARD TEST 1          *\n" \
		     "************************************\n";
	test_Pollard_start();
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "************************************\n" \
		     "*          POLLARD TEST 2          *\n" \
		     "************************************\n";
	test_Pollard_decode();
	std::cout << std::endl << std::endl << std::endl;


	std::cout << "************************************\n" \
		     "*             ECM TEST 1           *\n" \
		     "************************************\n";	
	test_ECM_start();
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "************************************\n" \
		     "*             ECM TEST 2           *\n" \
		     "************************************\n";
	test_ECM_decode();
	std::cout << std::endl << std::endl << std::endl;


	std::cout << "************************************\n" \
		     "*            QUAD TEST 1           *\n" \
		     "************************************\n";
	test_QS_start();
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "************************************\n" \
		     "*            QUAD TEST 2           *\n" \
		     "************************************\n";	
	test_QS_decode();
	std::cout << std::endl << std::endl << std::endl;

	return 0;
}
