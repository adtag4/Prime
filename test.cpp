#include "QS/QS.h"
#include "algorithm/algorithm.h"

#include <iostream>

int main()
{
	

	std::cout << "It works!" << std::endl;

	QS q(alg::INT("123456789987654321"), 10);
	q.Factor();

	return 0;
}
