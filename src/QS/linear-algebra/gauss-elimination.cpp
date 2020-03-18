#include "QS/linear-algebra/gauss-elimination.h"

//Performs a Gaussian elimination over M
void GaussElimination::Echelonize (Matrix& M)
{
	Matrix *ID_p = Matrix::GetIdentity(M.row_dim ());
	Matrix &ID = *ID_p;

	unsigned long int current_non_reduced_row = 0;
	unsigned long int current_pivot_idx;
	unsigned long int next_pivot_idx;

	for(unsigned long int col=0; col<M.col_dim (); ++col)
	{
		next_pivot_idx = (unsigned long int)-1;

		//Get the first row which has the bit at column col equals to 1
		for(unsigned long int i=current_non_reduced_row; i<M.row_dim (); ++i)
		{
			if(mpz_tstbit(M[i], col))
			{
				next_pivot_idx = i;
				break;
			}
		}

		//Now condidate row found, continue to next column
		if(next_pivot_idx == (unsigned long int)-1)
		{
			continue;
		}

		//The pivot is different from the current non reduced row, switch them!
		if(next_pivot_idx != current_non_reduced_row)
		{
			mpz_swap(M[next_pivot_idx], M[current_non_reduced_row]);
			mpz_swap(ID[next_pivot_idx], ID[current_non_reduced_row]);
		}
		current_pivot_idx = current_non_reduced_row;
		++current_non_reduced_row;

		//Reduce all the remaining rows with the pivot now located at
		for(unsigned long int i=current_non_reduced_row; i<M.row_dim (); ++i)
		{
			if(mpz_tstbit(M[i], col))
			{
				//XOR to eliminate the 1 in row i, column col.
				mpz_xor(M[i], M[i], M[current_pivot_idx]);
				mpz_xor(ID[i], ID[i], ID[current_pivot_idx]);
			}
		}
	}

	this->FillLinearRelations(M, ID);
	delete ID_p;

	is_echelonized = true;
}


void GaussElimination::FillLinearRelations (Matrix& M, Matrix& ID)
{
	if(ID.row_dim() < 1)
		return;

	long last_row_idx = ID.row_dim() - 1;
	int nb_linear_rel = 0;

	//compute how many rows are linearly dependent in the matrix M
	//the linear dependency is stored in the matrix ID
	while(last_row_idx >= 0 && mpz_cmp_ui(M[last_row_idx], 0) == 0)
	{
		--last_row_idx;
		++nb_linear_rel;
	}

	if(nb_linear_rel < 1)	//Should never happen in a normal QS configuration
		return;				//the number of rows must always be more than # columns

	last_row_idx = ID.row_dim() - 1;

	for(int i=0; i<nb_linear_rel; ++i)
	{
		this->linear_relations.push_back(mpz_class(ID[last_row_idx - i]));
	}
}


const vector<mpz_class>& GaussElimination::GetLinearRelations ()
{
	if(!this->is_echelonized)
		throw std::logic_error ("Must call Echelonize first");

	return this->linear_relations;
}


unsigned int GaussElimination::GetNbLinearRelations () const
{
	if(!this->is_echelonized)
		throw std::logic_error ("Must Call Echelonize first");

	return this->linear_relations.size ();
}