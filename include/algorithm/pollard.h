// pollard.h

#pragma once

#include "algorithm.h"

namespace alg
{

class PollardState : public alg::AlgStateData
{
	public:
		PollardState() = 0; //delete default ctor
		~PollardState();

		// use this one
		PollardState(alg::INT n); // for initial 
		PollardState(alg::INT n, alg::INT x, alg::INT, y, alg::INT d); // for ongoing
		PollardState(char *encoding) // for reversing encoding

		char	*encode()	override;
	protected:
	private:
		alg::INT	x;
		alg::INT	y;
		alg::INT	d;
		alg::INT	n;
};

class Pollard : public alg::Algorithm
{
	public:
		Pollard() = 0; // don't use default ctors silly
		~Pollard();

		// use this instead
		Pollard(PollardState& startState);

		void 		proceed()	override;
		bool 		foundFactor()	override;
		AlgStateData&	currentState()	override;
	protected:
	private:
		PollardState	current;
};

}
