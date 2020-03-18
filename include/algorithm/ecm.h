// ecm.h
// Elliptic Curve Factoring 

#pragma once

#include "algorithm/algorithm.h"

namespace alg
{

// gets modular inverse 
// 	sets dst with result (inverse or gcd)
//	ret true if inverse, false if gcd
bool inverse(alg::INT a, alg::INT n, alg::INT& dst);

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
		
		friend ostream& operator << (ostream& out, alg::PrePoint& data);
		friend istream& operator >> (istream& in,  alg::PrePoint& data);

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

		friend ostream& operator << (ostream& out, alg::EllipticCurve& data);
		friend istream& operator >> (istream& in,  alg::EllipticCurve& data);
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

		Point add(Point& p);
		Point operator+(Point& p);

		friend ostream& operator << (ostream& out, alg::Point& data);
		friend istream& operator >> (istream& in,  alg::Point& data);
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
		void encode(ostream& out) override;
		void decode(istream& in)  override;
};

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

// global namespace
ostream& operator << (ostream& out, alg::PrePoint& data);
istream& operator >> (istream& in,  alg::PrePoint& data);

ostream& operator << (ostream& out, alg::EllipticCurve& data);
istream& operator >> (istream& in,  alg::EllipticCurve& data);

ostream& operator << (ostream& out, alg::Point& data);
istream& operator >> (istream& in,  alg::Point& data);
