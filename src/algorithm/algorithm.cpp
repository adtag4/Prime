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

}
