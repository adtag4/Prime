// pollard.cpp

// should contain all relevant include statements for GMP
#include "algorithm/pollard.h"

#include <string>
namespace alg
{

/***********************************************
 *        PollardState Functions               *
 ***********************************************/

PollardState::PollardState(alg::INT n)
	: n_(n),
	  d_("1"),
	  x_("2"),
	  y_("2")
{
	// initial state of a pollard alg should be set up now
}

PollardState::PollardState(alg::INT n, alg::INT x, alg::INT y, alg::INT d)
	: n_(n),
	  d_(d),
	  x_(x),
	  y_(y)
{
	// values should be copied
}

// CODE ASSUMES THAT INPUT WAS GIVEN BY PollardState::encode()
PollardState::PollardState(char *encoding)
{
	uint32_t *iPtr = (uint32_t*)encoding;
	
	uint32_t tLen = *iPtr; // total len is first 4 bytes of encoded state
	
	uint32_t nLen = iPtr[1]; // next 4 bytes

	auto nPtr = encoding + 8;
	char tmp = nPtr[nLen];
	nPtr[nLen] = 0; // make null temporarily for string ops
	n_ = alg::INT(std::string(nPtr));
	nPtr[nLen] = tmp;
	
	nPtr = nPtr + nLen;
	iPtr = (uint32_t *)nPtr;
	nPtr = nPtr + 4;
	nLen = *iPtr;
	tmp = nPtr[nLen];
	nPtr[nLen] = 0;
	d_ = alg::INT(std::string(nPtr));
	nPtr[nLen] = tmp;

	nPtr = nPtr + nLen;
	iPtr = (uint32_t *)nPtr;
	nPtr = nPtr + 4;
	nLen = *iPtr;
	tmp = nPtr[nLen];
	nPtr[nLen] = 0;
	x_ = alg::INT(std::string(nPtr));
	nPtr[nLen] = tmp;

	// y is last, followed by a null so we don't need to worry about it
	nPtr = nPtr + nLen + 4;
	y_ = alg::INT(std::string(nPtr));
}

// encodes in format of TOTAL_LEN|nLEN|n|dLEN|d|xLEN|x|yLEN|y|\0
char *PollardState::encode()
{
	std::string nStr = n_.get_str();
	std::string dStr = d_.get_str();
	std::string xStr = x_.get_str();
	std::string yStr = y_.get_str();

	uint32_t nLen = static_cast<uint32_t>(nStr.length());
	uint32_t dLen = static_cast<uint32_t>(dStr.length());
	uint32_t xLen = static_cast<uint32_t>(xStr.length());
	uint32_t yLen = static_cast<uint32_t>(yStr.length());

	uint32_t totalLen = nLen + dLen + xLen + yLen + 16; // +16 for len of 4 lens (4 bytes each)
	
	char *msg = (char*)calloc(1, totalLen + 5); // +5 for totalLen length and null byte at end

	int q = 0; // current write location
	*((uint32_t *) msg + q) = totalLen;

	q = 4;
	*((uint32_t *) msg + q) = nLen;

	q = 8 + nLen;
	*((uint32_t *) msg + q) = dLen;

	q = 12 + nLen + dLen;
	*((uint32_t *) msg + q) = xLen;

	q = 16 + nLen + dLen + xLen;
	*((uint32_t *) msg + q) = yLen;

	q = 8;
	auto dst = msg + q;
	int i = 0;
	while(i < nLen)
	{
		dst[i] = nStr.c_str()[i];
		i++;
	}

	q = 12 + nLen;
	dst = msg + q;
	i = 0;
	while(i < dLen)
	{
		dst[i] = dStr.c_str()[i];
		i++;
	}

	q = 16 + nLen + dLen;
	dst = msg + q;
	i = 0;
	while(i < xLen)
	{
		dst[i] = xStr.c_str()[i];
		i++;
	}

	q = 20 + nLen + dLen + xLen;
	dst = msg + q;
	i = 0;
	while(i < yLen)
	{
		dst[i] = yStr.c_str()[i];
		i++;
	}

	return msg;

}

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
