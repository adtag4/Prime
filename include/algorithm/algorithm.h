// Algorithm templated stuffs

#pragma once

// according to good practice: this should not be here.  For convenience since it all uses these...:)
#include <gmp.h>
#include <gmpxx.h>

#include <iostream>


namespace alg
{

// typedef bc INT is better objectively
typedef mpz_class INT;

class AlgStateData // abstract class - varies per alg
{
	public:
		AlgStateData() {};
		~AlgStateData() {};

		// provide char array encoding of state
		friend std::ostream& operator << (std::ostream& out, alg::AlgStateData& data);
		friend std::istream& operator >> (std::istream& in,  alg::AlgStateData& data);
	protected:
		virtual void encode(std::ostream& out) = 0;
		virtual void decode(std::istream& in)  = 0;
	private:
};

class Algorithm // abstract class - varies per alg
{
	public:
		Algorithm() {};
		//Algorithm(InitStateData initialState)
		~Algorithm() {};

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
void runXTimes(std::ostream& out, int x, A a)
{
	int i = 0;
	while (i < x)
	{
		a.proceed();
		i++;

		if(a.foundFactor())
		{
			break;
		}
	}
	out << a.currentState();
}

// gets mod inverse (return true) or gcd (return false)
//   result (inv or gcd) stored in dst
bool inverse(alg::INT a, alg::INT n, alg::INT& dst);
alg::INT gcd(alg::INT a, alg::INT b);

}

std::ostream& operator << (std::ostream& out, alg::AlgStateData& data);
std::istream& operator << (std::istream& in,  alg::AlgStateData& data);
