// pollard.h

#pragma once

#include "algorithm.h"

namespace alg
{

class PollardState : public alg::AlgStateData
{
	public:
		PollardState(); //delete default ctor
		~PollardState();

		// use this one
		PollardState(alg::INT n); // for initial 
		PollardState(alg::INT n, alg::INT x, alg::INT y, alg::INT d); // for ongoing
		PollardState(char *encoding); // for reversing encoding

		char	*encode()	override;
		alg::INT	x_;
		alg::INT	y_;
		alg::INT	d_;
		alg::INT	n_;
};

class Pollard : public alg::Algorithm
{
	public:
		Pollard(); // don't use default ctors silly
		~Pollard();

		// use this instead
		Pollard(PollardState& startState);

		void		proceed()	override;
		bool 		foundFactor()	override;
		AlgStateData&	currentState()	override;
	protected:
	private:
		virtual alg::INT	g(alg::INT x, alg::INT n);
		PollardState		current;
};

}
