// ecm.h
// Elliptic Curve Factoring 

#pragma once

#include "algorithm/algorithm.h"

namespace alg
{


class ECM : public alg::Algorithm
{
	public:
		ECM() {}; // DONT USE THIS... but c++ hates deleted default constructors :(
		~ECM() {};
		ECM(ECMState& startState);

		// functionality
		void		proceed()	override; // finds next multiple of P by adding P + newP

		bool		foundFactor()	override; // determines if current_.factor_ is 0 or a factor

		AlgStateData&	currentState()	override; // returns ECMState current_

		ECMState	current_;
};


}
