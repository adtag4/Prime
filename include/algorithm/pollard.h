// pollard.h

#pragma once

#include "algorithm.h"

namespace alg
{

class Pollard : public alg::Algorithm
{
	public:
		Pollard() {}; // don't use default ctors silly
		~Pollard() {};

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
