// created to adapt QS code - all is from smooth-base.h originally

#include "QS/smooth-base.h"

mpz_class SmoothBase::GetBase()
{
	return this->B;
}


mpz_class SmoothBase::GetRSAModulus()
{
	return this->N;
}

void SmoothBase::GetSmoothnessBase(mpz_class& ret_base, mpz_class& N)
{
	mpfr_t f_N, log_N, log_log_N;
	mpz_t base_mpz;
	mpz_init(base_mpz);

	mpfr_init(f_N); mpfr_init(log_N); mpfr_init(log_log_N);

	mpfr_set_z(f_N, N.get_mpz_t(), MPFR_RNDU);		//f_N = N
	mpfr_log(log_N, f_N, MPFR_RNDU); 				//log_N = log(N)
	mpfr_log(log_log_N, log_N, MPFR_RNDU); 			//log_log_N = log(log(N))

	mpfr_mul(f_N, log_N, log_log_N, MPFR_RNDU); 	//f_N = log(N) * log(log(N))
	mpfr_sqrt(f_N, f_N, MPFR_RNDU); 				//f_N = sqrt(f_N)

	mpfr_div_ui(f_N, f_N, 2, MPFR_RNDU);  			//f_N = f_N/2
	mpfr_exp(f_N, f_N, MPFR_RNDU);					//f_N = e^f_N

	mpfr_get_z(base_mpz, f_N, MPFR_RNDU);
	ret_base = mpz_class(base_mpz);

	mpfr_clears(f_N, log_N, log_log_N, NULL);
	mpz_clear(base_mpz);
}


void SmoothBase::SievePrimesWhereNQuadraticResidue()
{
	Erastosthenes sieve (false);
	sieve.GetPrimes_QuadraticResidue(this->primes, this->B.get_ui() ,this->N);
}


void SmoothBase::ComputeRoots()
{
	mpz_class root, r2;
	mpz_t tmp_p, tmp_root, tmp_N;
	mpz_init(tmp_p); mpz_init(tmp_root); mpz_init(tmp_N);

	mpz_set(tmp_N, N.get_mpz_t());

	for(int i=0; i<this->primes.size(); ++i)
	{
		mpz_set_ui(tmp_p, (unsigned long int) this->primes[i]);
		mpz_sqrtm(tmp_root, tmp_N, tmp_p);	/* calculate the modular root */

		root = mpz_class(tmp_root);
		this->roots_1.push_back(root.get_ui());		//root 1

		root = -1 * root;
		root += this->primes[i];		//Get a positive value, % returns a negative value
		//root %= this->primes[i];

		assert(root >= 0);

		this->roots_2.push_back(root.get_ui());
	}

	mpz_clears(tmp_p, tmp_root, tmp_N, NULL);
}


void SmoothBase::Setup()
{
	this->SievePrimesWhereNQuadraticResidue();
	this->ComputeRoots();
}
