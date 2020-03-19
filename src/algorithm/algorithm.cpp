// algorithm.cpp

#include "algorithm/algorithm.h"

namespace alg
{

// helper for inverse - based on geeksforgeeks.org/multiplicative-inverse-under-modulo-m
alg::INT extendedEuclid(alg::INT a, alg::INT n, alg::INT& x, alg::INT& y)
{
	if(a == alg::INT("0"))
	{
		x = 0_mpz;
		y = 1_mpz;
		return n;
	}
	
	alg::INT xStored("0");
	alg::INT yStored("0");
	alg::INT g = extendedEuclid(n % a, a, xStored, yStored);

	x = yStored - ((n / a) * xStored);
	y = xStored;

	return g;
}

// my own modified extended euclidean algorithm
// based on geeksforgeeks.org/multiplicative-inverse-under-modulo-m
//
// return values:
//	TRUE: inverse of a (mod n) is in dst
//	FALSE: gcd(a, n) is in dst
bool inverse(alg::INT a, alg::INT n, alg::INT&dst)
{
	alg::INT x("0");
	alg::INT y("0");

	alg::INT g = extendedEuclid(a, n, x, y);
	if(g == alg::INT("1"))
	{
		dst = ((x % n) + n) % n; // in case negative
		return true;
	}
	else
	{
		dst = g; // send the gcd instead!
		return false;
	}
}

// basic recursive euclid algorithm - based on geeksforgeeks.org/euclidean-algorithms-basic-and-extended
alg::INT gcd(alg::INT a, alg::INT b)
{
	if(a == alg::INT("0"))
	{
		return b;
	}
	return alg::gcd(b%a, a);
}

std::ostream& operator << (std::ostream& out, alg::AlgStateData& data)
{
	data.encode(out);
	return out;
}

std::istream& operator >> (std::istream& in, alg::AlgStateData& data)
{
	data.decode(in);
	return in;
}

void QuadSieveState::encode(std::ostream& out)
{
	out << n_ << ' ' << nbLinRels_ << ' ' << factor_;
}

void QuadSieveState::decode(std::istream& in)
{
	in >> n_ >> nbLinRels_ >> factor_;
}

/***********************************************
 *        PollardState Functions               *
 ***********************************************/

PollardState::PollardState(alg::INT n)
	: n_(n),
	  d_("1"),
	  x_("2"),
	  y_("2")
{
	// initial state of a pollard alg should be set up now
}

PollardState::PollardState(alg::INT n, alg::INT x, alg::INT y, alg::INT d)
	: n_(n),
	  d_(d),
	  x_(x),
	  y_(y)
{
	// values should be copied
}


void PollardState::encode(std::ostream& out)
{
	out << x_ << ' ' << y_ << ' ' << d_ << ' ' << n_;
}


void PollardState::decode(std::istream& in)
{
	in >> x_ >> y_ >> d_ >> n_;
}

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
//		std::cout << "You done messed up A-Aron - curve_==nullptr in Point::add()" << std::endl;

		throw p; // seriously, you shouldn't end up here.  Don't forget to set the curve
	}
	
	alg::INT numerator("0");
	alg::INT denominator("0");

	// adding points:
	if((x_ == p.x_) && (y_ == p.y_)) // adding P+P
	{
		// DEBUG
//		std::cout << "\tAdding point to self." << std::endl;

		numerator 	= alg::INT("3") * x_ * x_ + curve_->b_;
		denominator 	= alg::INT("2") * y_;

	}
	else // adding different points
	{
		// DEBUG
//		std::cout << "\tAdding different points." << std::endl;

		numerator	= p.y_ - y_;
		denominator	= p.x_ - x_;
	}

	// ensure positive:
	numerator = (numerator + curve_->n_) % curve_->n_;
	denominator = (denominator + curve_->n_) % curve_->n_;

	//DEBUG
//	std::cout << "\tM num: " << numerator << std::endl;
//	std::cout << "\tM den: " << denominator << std::endl;
	
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
///		std::cout << "\tFound inverse, m: ";

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
//		std::cout << "\tNo inverse, gcd: " << inv << std::endl;

		throw inv; // the gcd
	}

}

// hehehe - <insert evil laugh here>
Point Point::operator + (Point& p)
{
	// DEBUG
//	std::cout << "Adding " << *this << " + " << p << std::endl;
	
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

bool ECMState::isValid()
{
	alg::INT left = p_.y_ * p_.y_;
	alg::INT right = p_.x_ * p_.x_ * p_.x_ + curve_.b_ * p_.x_ + curve_.c_;
	if(left != right)
	{
		return false;
	}
	if(curve_.c_ == alg::INT("0"))
	{
		return false;
	}
	return true;
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
