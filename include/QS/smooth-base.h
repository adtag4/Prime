/*
 * smooth-base.h
 *
 *  Created on: Jan 6, 2013
 *      Author: sky
 *      Copyright martani 2013
 */
#pragma once 

#ifndef SMOOTH_BASE_H_
#define SMOOTH_BASE_H_

#include <vector>
#include <climits>
#include <sstream>
#include <gmp.h>
#include <gmpxx.h>
#include <mpfr.h>
#include <cassert>

#include "QS/eratosthenes.h"
#include "QS/gmp-patch.h"

extern "C" int mpz_sqrtm(mpz_t q, const mpz_t n, const mpz_t p);

class SmoothBase
{
private:

	//The smooth base upper bound
	mpz_class B;

	//The RSA modulus
	mpz_class N;

	//Computes the smoothness base to which all the primes used in sieving
	//are in [0..rest_base]
	static void GetSmoothnessBase(mpz_class& ret_base, mpz_class& n);

	SmoothBase(){};

	void SievePrimesWhereNQuadraticResidue();

	void ComputeRoots();

public:

	std::vector<unsigned long int> primes;
	std::vector<unsigned long int> roots_1;
	std::vector<unsigned long int> roots_2;

	SmoothBase(mpz_class modulus)
	{
		N = modulus;
		GetSmoothnessBase(B, modulus);

		//check if the base is less than ULONG_MAX, since all the primes in the
		//smooth base are of type ulong int.
		if(B >= ULONG_MAX)
		{
			std::ostringstream msg;
			msg << "Smooth base too large (=" << B.get_str(10)
				<< ")! Only \"unsigned long\" primes are supported for now. (ULONG_MAX="
				<< ULONG_MAX << ")";
			throw std::length_error(msg.str());
		}
	}

	//Sieves for the primes in the base and computes the roots of N mod these primes
	void Setup();

	mpz_class GetBase();
	mpz_class GetRSAModulus();
};



#endif /* SMOOTH_BASE_H_ */
