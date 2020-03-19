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
void runXTimes(std::ostream& out, int x, A& a)
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
		if((i % 100) == 0)
		{
			std::cout << "alg::runXTimes has run " << i << " times" << std::endl;
		}
	}
	out << a.currentState() << std::endl;
}

// gets mod inverse (return true) or gcd (return false)
//   result (inv or gcd) stored in dst
bool inverse(alg::INT a, alg::INT n, alg::INT& dst);
alg::INT gcd(alg::INT a, alg::INT b);

std::ostream& operator << (std::ostream& out, alg::AlgStateData& data);
std::istream& operator >> (std::istream& in,  alg::AlgStateData& data);


// Here are states for every algorithm: commonly incuded by user nodes (but they dont need algorithm objects)
class QuadSieveState : public AlgStateData
{
	public:
		QuadSieveState() {}; // DONT USE
		~QuadSieveState() {};
		
		QuadSieveState(alg::INT n, int nbLinRels)
			: n_(n),
			  nbLinRels_(nbLinRels),
			  factor_("0")
		{};

		alg::INT	n_;
		int		nbLinRels_;
		alg::INT	factor_;
	protected:
		void encode(std::ostream& out) override;
		void decode(std::istream& in)  override;

};

class PollardState : public alg::AlgStateData
{
	public:
		PollardState() {}; //delete default ctor
		~PollardState() {};

		// use this one
		PollardState(alg::INT n); // for initial 
		PollardState(alg::INT n, alg::INT x, alg::INT y, alg::INT d); // for ongoing

		alg::INT	x_;
		alg::INT	y_;
		alg::INT	d_;
		alg::INT	n_;

	protected:
		void encode(std::ostream& out) override;
		void decode(std::istream& in)  override;
};

class PrePoint
{
	public:
		PrePoint() {}; // never use default ctors!
		~PrePoint() {};

		PrePoint(alg::INT x, alg::INT y)	
			: x_(x),
	  		  y_(y)
			 {
			 };

		alg::INT x_;
		alg::INT y_;
		
		friend std::ostream& operator << (std::ostream& out, alg::PrePoint& data);
		friend std::istream& operator >> (std::istream& in,  alg::PrePoint& data);

};

class EllipticCurve
{
	public:
		EllipticCurve() {}; // still never use default ctors!
		~EllipticCurve() {};

		EllipticCurve(PrePoint& p, alg::INT b, alg::INT n);
		EllipticCurve(alg::INT b, alg::INT c, alg::INT n);

		alg::INT b_;
		alg::INT c_;
		alg::INT n_;

		friend std::ostream& operator << (std::ostream& out, alg::EllipticCurve& data);
		friend std::istream& operator >> (std::istream& in,  alg::EllipticCurve& data);
};

class Point
{
	public:
		Point() {}; // never use default ctors!
		~Point() {};

		Point(alg::INT x, alg::INT y);

		alg::INT x_;
		alg::INT y_;

		void setCurve(EllipticCurve *curve);
		EllipticCurve* curve_;

		Point add(const Point& p);
		Point operator+(Point& p);

		friend std::ostream& operator << (std::ostream& out, alg::Point& data);
		friend std::istream& operator >> (std::istream& in,  alg::Point& data);
};



class ECMState : public AlgStateData
{
	public:
		ECMState() {};
		~ECMState() {};

		ECMState(Point p, Point p2, EllipticCurve curve);
		
		Point		p_;	 // basis point
		Point		newP_;	 // product of basis (coef irrelevant)
		EllipticCurve	curve_;	 // curve point is on
		alg::INT	factor_; // for if a factor is found (0 if not)

	protected:
		void encode(std::ostream& out) override;
		void decode(std::istream& in)  override;
};

std::ostream& operator << (std::ostream& out, alg::PrePoint& data);
std::istream& operator >> (std::istream& in,  alg::PrePoint& data);

std::ostream& operator << (std::ostream& out, alg::EllipticCurve& data);
std::istream& operator >> (std::istream& in,  alg::EllipticCurve& data);

std::ostream& operator << (std::ostream& out, alg::Point& data);
std::istream& operator >> (std::istream& in,  alg::Point& data);

}
