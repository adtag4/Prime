/*
 * gauss-elimination.h
 *
 *  Created on: Jan 17, 2013
 *      Author: sky
 */

#pragma once

#ifndef GAUSS_ELIMINATION_H_
#define GAUSS_ELIMINATION_H_

#include <gmp.h>
#include "QS/linear-algebra/matrix.h"
#include <vector>

class GaussElimination {
private:

	//Fills the vector linear_relations after Echelonize is called
	void FillLinearRelations (Matrix& M, Matrix& ID);
	bool is_echelonized;
	std::vector<mpz_class> linear_relations;

public:

	GaussElimination () : is_echelonized (false) {}

	~GaussElimination ()
	{
		//if(linear_relations != NULL)
			//delete [] linear_relations;
	}

	//Performs a Gaussian elimination over the matrix M
	void Echelonize (Matrix& M);

	//Returns the linear relations resulted from the echelon form of the Matrix M
	const std::vector<mpz_class>& GetLinearRelations ();

	unsigned int GetNbLinearRelations () const;
};




#endif /* GAUSS_ELIMINATION_H_ */
