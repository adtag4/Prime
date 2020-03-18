// Algorithm templated stuffs

#pragma once

// according to good practice: this should not be here.  For convenienve :)
#include <gmp.h>
#include <gmpxx.h>



namespace alg
{

// typedef bc INT is better objectively
typedef mpz_class INT;

class AlgStateData // abstract class - varies per alg
{
	public:
		AlgStateData();
		~AlgStateData();

		// provide char array encoding of state
		virtual char *encode() = 0;
	protected:
	private:
};

class Algorithm // abstract class - varies per alg
{
	public:
		Algorithm();
		//Algorithm(InitStateData initialState)
		~Algorithm();

		// run one step of it
		virtual void proceed() = 0;

		// evaluate for completion
		virtual bool foundFactor() = 0;

		// get results (final or intermediate)
		virtual AlgStateData& currentState() = 0;

	protected:
	private:
		// your class will have it's subclass of AlgStateData as a member
};

// returns encoded result of running algorithm x times
template <typename A>
char *runXTimes(int x, A a)
{
	int i = 0;
	for(while i < x)
	{
		a.proceed();
		i++;

		if(a.foundFactor)
		{
			return a.currentState().encode();
		}
	}
	return a.currentState.encode();
}

}
