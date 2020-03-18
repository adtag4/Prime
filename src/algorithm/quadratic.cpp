// quadratic.cpp

#include "algorithm/algorithm.h"
#include "algorithm/quadratic.h"

namespace alg
{

bool QuadSieve::foundFactor()
{
	if(qsMachine_.GetFactor1() == 1 || qsMachine_.GetFactor1() == current_.n_)
	{
		return false;
	}
	else
	{
		return true;
	}
}


AlgStateData& QuadSieve::currentState()
{
	return current_;
}





}
