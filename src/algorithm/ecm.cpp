// ecm.cpp

#include "algorithm/algorithm.h"
#include "algorithm/ecm.h"

namespace alg
{

/**************************************
 *         Point functions            *
 **************************************/

Point::Point(alg::INT x, alg::INT y)
	: x_(x),
	  y_(y)
{
	curve_ = nullptr;
}

void Point::setCurve(EllipticCurve *curve)
{
	curve_ = curve;
}

Point Point::add(const Point& p)
{
	if(curve_ == nullptr)
	{
		//DEBUG
		std::cout << "You done messed up A-Aron - curve_==nullptr in Point::add()" << std::endl;

		throw p; // seriously, you shouldn't end up here.  Don't forget to set the curve
	}
	
	alg::INT numerator("0");
	alg::INT denominator("0");

	// adding points:
	if((x_ == p.x_) && (y_ == p.y_)) // adding P+P
	{
		// DEBUG
		std::cout << "\tAdding point to self." << std::endl;

		numerator 	= alg::INT("3") * x_ * x_ + curve_->b_;
		denominator 	= alg::INT("2") * y_;

	}
	else // adding different points
	{
		// DEBUG
		std::cout << "\tAdding different points." << std::endl;

		numerator	= p.y_ - y_;
		denominator	= p.x_ - x_;
	}

	// ensure positive:
	numerator = (numerator + curve_->n_) % curve_->n_;
	denominator = (denominator + curve_->n_) % curve_->n_;

	//DEBUG
	std::cout << "\tM num: " << numerator << std::endl;
	std::cout << "\tM den: " << denominator << std::endl;
	
	if(denominator == alg::INT("0")) // m is infinite :(
	{
		throw 0; 
		// if infinite, result is the point infinity which can't be done anything with
		// as it is the identity of the group, and means you're in a cycle and should 
		// stop evaluating the ECM thread (it doesn't do to repeat work)
	}

	alg::INT inv("0");
	if(alg::inverse(denominator, curve_->n_, inv)) // inverse exists, calculate next point
	{
		// DEBUG
		std::cout << "\tFound inverse, m: ";

		alg::INT m = (numerator * inv) % curve_->n_;

		//DEBUG
		std::cout << m << std::endl;


		alg::INT newX = (m * m) - x_ - p.x_;
		newX = ((newX % curve_->n_) + curve_->n_) % curve_->n_ ;
		alg::INT newY = (m * (x_ - newX)) - y_;
		newY = ((newY % curve_->n_) + curve_->n_) % curve_->n_;
		Point q(newX, newY);
		q.setCurve(curve_);
		return q;
	}
	else // inverse doesn't exist, throw the new factor
	{
		// DEBUG
		std::cout << "\tNo inverse, gcd: " << inv << std::endl;

		throw inv; // the gcd
	}

}

// hehehe - <insert evil laugh here>
Point Point::operator + (Point& p)
{
	// DEBUG
	std::cout << "Adding " << *this << " + " << p << std::endl;
	
	return add(p);
}

/*****************************************
 *     EllipticCurve functions           *
 *****************************************/

// construct curve based on input point and b (and n)
EllipticCurve::EllipticCurve(PrePoint& p, alg::INT b, alg::INT n)
{
	/* Elliptic curves are of format y^2 = x^3 + b*x + c (mod n)
	 *   We have simplified finding a curve and point on the 
	 *   curve by selecting a point and b, and solving for c
	 *	(conceptual src: Trappe/Washington 2nd ed.)
	 */

	alg::INT left = p.y_ * p.y_;
	alg::INT right = p.x_ * p.x_ * p.x_ + b * p.x_;
	alg::INT testC = (((left - right) % n) + n) % n; // account for negative values...

	n_ = n;
	b_ = b;
	c_ = testC;
}

EllipticCurve::EllipticCurve(alg::INT b, alg::INT c, alg::INT n)
	: b_(b),
	  c_(c),
	  n_(n)
{
	
}


/******************************************
 *          ECMState functions            *
 ******************************************/

ECMState::ECMState(Point p, Point p2, EllipticCurve curve)
	: p_(p),
	  newP_(p2),
	  curve_(curve),
	  factor_("0")
{
	p_.setCurve(&curve_);
	newP_.setCurve(&curve_);
}

void ECMState::encode(std::ostream& out)
{
	out << p_ << ' ' << newP_ << ' ' << curve_ << ' ' << factor_ << '\n';
}

void ECMState::decode(std::istream& in)
{
	in >> p_ >> newP_;
	in >> curve_;
	in >> factor_;
	p_.setCurve(&curve_);
	newP_.setCurve(&curve_);
}


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

std::ostream& operator << (std::ostream& out, alg::PrePoint& data)
{
	out << data.x_ << ' ' << data.y_;
	return out;
}

std::istream& operator >> (std::istream& in, alg::PrePoint& data)
{
	in >> data.x_ >> data.y_;
	return in;
}

std::ostream& operator << (std::ostream& out, alg::EllipticCurve& data)
{
	out << data.b_ << ' ' << data.c_ << ' ' << data.n_;
	return out;
}

std::istream& operator >> (std::istream& in, alg::EllipticCurve& data)
{
	in >> data.b_ >> data.c_ >> data.n_;
	return in;
}

std::ostream& operator << (std::ostream& out, alg::Point& data)
{
	out << data.x_ << ' ' << data.y_;
	return out;
}

std::istream& operator >> (std::istream& in, alg::Point& data)
{
	in >> data.x_ >> data.y_;
	return in;
}

}
