// quadratic.cpp

#include "algorithm/algorithm.h"
#include "algorithm/quadratic.h"

namespace alg
{

char *QuadSieveState::encode()
{
	//TODO
}

bool QuadSieve::foundFactor()
{
	if(qsMachine_.GetFactor1() == 1 || qsMachine.GetFactor1() == current_.n_)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void QuadSieveState::encode(ostream& out)
{
	out << n_ << ' ' << nbLinRels_ << ' ' << factor_;
}

void QuadSieveState::decode(istream& in)
{
	in >> n_ >> nbLinRels_ >> factor_;
}

AlgStateData& QuadSieve::currentState()
{
	return current_;
}





}
