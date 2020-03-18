#include "algorithm/algorithm.h"
#include "algorithm/pollard.h"
#include "algorithm/ecm.h"
#include "algorithm/quadratic.h"

//#include <iostream>
//#include <string>
//#include <sstream>


int main()
{
	

	std::cout << "It works!" << std::endl;
	
	alg::PrePoint pp(5_mpz, 6_mpz);
	alg::EllipticCurve ec(pp, 3_mpz, 2019_mpz);

	std::cout << pp << std::endl;
	std::cout << ec << std::endl;
	
	return 0;
}
