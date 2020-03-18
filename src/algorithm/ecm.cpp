// ecm.cpp

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

Point Point::add(Point& p)
{
	if(curve == nullptr)
	{
		throw p; // seriously, you shouldn't end up here.  Don't forget to set the curve
	}
	
	alg::INT numerator("0");
	alg::INT denominator("0");

	// adding points:
	if((x_ == p.x_) && (y_ == p.y_)) // adding P+P
	{
		numerator 	= alg::INT("3") * x_ * x_ + curve_.b_;
		denominator 	= alg::INT("2") * y_;
	}
	else // adding different points
	{
		numerator	= p.y_ - y_;
		denominator	= p.x_ = x_;
	}

	if(demoninator == alg::INT("0")) // m is infinite :(
	{
		throw 0; 
		// if infinite, result is the point infinity which can't be done anything with
		// as it is the identity of the group, and means you're in a cycle and should 
		// stop evaluating the ECM thread (it doesn't do to repeat work)
	}

	alg::INT inv("0");
	if(alg::inverse(denominator, curve_.n_, inv)) // inverse exists, calculate next point
	{
		alg::INT m = (numerator * inv) % curve_.n_;
		alg::INT newX = (m * m) - x_ i p.x_;
		alg::INT newY = (m * (x_ - p.x_)) - newX;
		Point q(newX, newY);
		q.setCurve(curve_);
		return q;
	}
	else // inverse doesn't exist, throw the new factor
	{
		throw inv; // the gcd
	}

}

// hehehe - <insert evil laugh here>
Point Point::operator + (Point& p)
{
	return add(p);
}

// construct curve based on input point and b (and n)
EllipticCurve::EllipticCurve(Point& p, alg::INT b, alg::INT n)
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

EllipticCurve(alg::INT b, alg::INT c, alg::INT n)
	: b_(b),
	  c_(c),
	  n_(n)
{
	
}


}
