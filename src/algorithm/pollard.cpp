// pollard.cpp

// should contain all relevant include statements for GMP
#include "algorithm/pollard.h"

#include <string>
namespace alg
{

/*****************************************
 *           Pollard functions           *
 *****************************************/
Pollard::Pollard(PollardState& startState)
	: current(startState)
{
	// should be good
}

// Allows easy changing of function used in algorithm - just override in an inherited Pollard2
alg::INT Pollard::g(alg::INT x, alg::INT n)
{
	return (x * x - 1) % n;
}

void Pollard::proceed()
{
	// test for futility first
	if(current.d_ == current.n_)
	{
		return;
	}

	// test for completeness
	if(foundFactor())
	{
		return;
	}


	current.x_ = g(current.x_, current.n_);
	current.y_ = g(g(current.y_, current.n_), current.n_);
	if(current.x_ > current.y_)
	{
		current.d_ = alg::gcd((current.x_ - current.y_), current.n_);
	}
	else
	{
		current.d_ = alg::gcd((current.y_ - current.x_), current.n_);
	}
}

bool Pollard::foundFactor()
{
	if((current.d_ == current.n_) || (current.d_ == alg::INT("1")))
	{
		return false;
	}
	return true;
}

AlgStateData& Pollard::currentState()
{
	return current;
}
}
