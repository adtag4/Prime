#include "QS/linear-algebra/matrix.h"

Matrix *Matrix::GetIdentity (unsigned long int nb_rows)
{
	Matrix *M = new Matrix (nb_rows, nb_rows);
	M->next_available_row = nb_rows;

	for(unsigned long int i=0; i<nb_rows; ++i)
	{
		//Init a nb_rows bit vector, set all entries to 0
		mpz_init2((*M)[i], nb_rows);

		//Set bit on diagonal at row i to 1
		mpz_setbit((*M)[i], i);
	}

	return M;
}


bool Matrix::PushExponentVector (SmoothNumber& n)
{
	//Is there more room to push the row?
	if(this->next_available_row >= this->n_rows)
		return false;

	mpz_init_set(this->_A[this->next_available_row], n.GetExponentVector ());
	this->next_available_row++;

	return true;
}

bool Matrix::PushRow(const mpz_t r)
{
	if(this->next_available_row >= this->n_rows)
		return false;

	mpz_init_set(this->_A[this->next_available_row], r);
	this->next_available_row++;

	return true;
}