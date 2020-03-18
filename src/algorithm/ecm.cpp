// ecm.cpp

#include "algorithm/algorithm.h"
#include "algorithm/ecm.h"

namespace alg
{


/******************************************
 *             ECM functions              *
 ******************************************/

ECM::ECM(ECMState& startState)
	: current_(startState)
{
	
}

void ECM::proceed()
{
	//DEBUG
	std::cout << "Start of proceed(): " << currentState() << std::endl;

	if(current_.factor_ == alg::INT("0"))
	{
		try
		{
			//DEBUG
			std::cout << "base: " << current_.p_ << std::endl;
			std::cout << "oldP: " << current_.newP_ << std::endl;
			
			current_.newP_ = current_.newP_ + current_.p_;
			
			//DEBUG
			std::cout << "base: " << current_.p_ << std::endl;
			std::cout << "newP: " << current_.newP_ << std::endl << std::endl;
		}
		catch (alg::INT factor)
		{
			current_.factor_ = factor;
			return;
		}
		catch (int stop)
		{
			// if here, in a cycle
			current_.factor_ = alg::INT("-1"); // -1 means failure
			return;
		}
		//DEBUG
		std::cout << "No error/factor caught" << std::endl;
	}

	//DEBUG
	std::cout << "End of proceed(): " << currentState() << std::endl;
}

bool ECM::foundFactor()
{
	if(current_.factor_ == 0)
	{
		return false; // not done
	}
	return true; // done either success or failure
}

AlgStateData& ECM::currentState()
{
	return current_;
}

}
