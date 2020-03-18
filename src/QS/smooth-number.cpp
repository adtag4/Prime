// for adapting QS code - all originally from smooth-number.h

#include "QS/smooth-number.h"

void SmoothNumber::InitWithoutExponentVector (mpz_class init_x, mpz_class modulus)
{
	this->X = init_x;
	if(!this->_is_initialized)
	{
		mpz_init(this->x_squared);
		this->_is_initialized = true;
	}

	//Set x_squared to X*X - N
	mpz_pow_ui(this->x_squared, X.get_mpz_t (), 2);
	mpz_sub(this->x_squared, this->x_squared, modulus.get_mpz_t ());

	this->smooth_base_size = 0;
	this->modulus_N = modulus;
}

const mpz_t& SmoothNumber::GetExponentVector ()
{
	return this->exponent_vector;
}

mpz_class SmoothNumber::GetXSquared () const
{
	return mpz_class (this->x_squared);
}
//const mpz_t& SmoothNumber::GetNonZeroExponentsVector () const
//{
//	return this->non_zeor_exponents_vector;
//}