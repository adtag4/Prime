// quadratic.cpp

#include "algorithm/algorithm.h"
#include "algorithm/quadratic.h"

#include "../QS/QS.C"
#include "../QS/eratosthenes.C"
#include "../QS/gmp-patch.c"
#include "../QS/math-utils.C"

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

void QuadSieveState::encode(std::ostream& out)
{
	out << n_ << ' ' << nbLinRels_ << ' ' << factor_;
}

void QuadSieveState::decode(std::istream& in)
{
	in >> n_ >> nbLinRels_ >> factor_;
}

AlgStateData& QuadSieve::currentState()
{
	return current_;
}





}
