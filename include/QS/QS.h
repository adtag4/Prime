/*
 * QS.h
 *
 *  Created on: Feb 21, 2013
 *      Author: sky
 */
#pragma once

#ifndef QS_H_
#define QS_H_

#include <gmp.h>
#include <gmpxx.h>

#include "QS/eratosthenes.h"
#include "QS/utils.h"
#include "QS/debug-cout.h"
#include "QS/math-utils.h"
#include "QS/smooth-base.h"
#include "QS/smooth-number.h"
#include "QS/linear-algebra/matrix.h"
#include "QS/linear-algebra/gauss-elimination.h"



#define MAX_NB_ADDITIONAL_LINEAR_RELS		20
#define SIEVING_STEP		50000	//Sieve for SIEVING_STEP numbers at a time


class QS {

protected:
	mpz_class _N, p, q;
	unsigned additional_linear_relations;
	unsigned long smooth_base_size;

	//The smooth numbers to be found during the Sieving step
	std::vector<SmoothNumber> final_smooth_numbers;

	//The exponents std::vectors' matrix. Filled during the Sieving step
	Matrix _M;

	//The carried back solutions of the dependent rows of the matrix _M
	//These are the solutions to the system represented by _M in GF(2)
	std::vector<mpz_class> linear_relations;
	GaussElimination gauss;


	virtual void SetupSmoothBase (SmoothBase& B);
	virtual void Sieve ();
	void PerformeGaussianElimination ();
	void FactorUsingLinearRelations ();

	void SetFactors (mpz_class f1, mpz_class f2);

	bool computation_completed;

	DebugCout dcout;

public:

	QS (mpz_class N);
	QS (mpz_class N, unsigned nb_additional_linear_relations);
	virtual ~QS () { ; }

	void Factor();

	mpz_class GetFactor1();
	mpz_class GetFactor2();

};


//#include "../../src/QS/QS.C"
#endif /* QS_H_ */
