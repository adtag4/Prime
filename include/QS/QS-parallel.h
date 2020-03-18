/*
 * QS-parallel.h
 *
 *  Created on: Mar 5, 2013
 *      Author: sky
 */

#pragma once

#ifndef QS_PARALLEL_H_
#define QS_PARALLEL_H_

#include <omp.h>
#include "QS/QS.h"

class QSParallel : public QS {

private:
	unsigned nb_threads;

	virtual void Sieve ();


public:

	QSParallel (mpz_class N) : QS (N)
	{
		this->nb_threads = omp_get_max_threads ();
	}

	QSParallel (mpz_class N, unsigned nb_additional_linear_relations)
	: QS (N, nb_additional_linear_relations)
	{
		this->nb_threads = omp_get_max_threads ();
	}

	/**
	 * Sets the number of threads to use in the sieving step.
	 * If this method is not called, the max available number of threads
	 * on this processor are used.
	 */
	void setNumThreads (unsigned nbThreads);
	virtual ~QSParallel () { ; }
};






#endif /* QS_PARALLEL_H_ */
