
#include "QS/utils.h"

namespace Utils
{

static void dumpMatrixAsPbmImage(const Matrix& A, std::string output_file_name)
{
	char buffer[512];
	unsigned char output_byte = 0;

	FILE *outStream = fopen(output_file_name.c_str (), "wb");

	//magic PBM header
#ifdef __LP64__	//64 bit machine
	sprintf(buffer, "P4\n# matrix size(%lu, %lu)\n%lu %lu\n", A.row_dim (), A.col_dim (), A.col_dim (), A.row_dim ());
#else			//32 bit machine
	sprintf(buffer, "P4\n# matrix size(%u, %u)\n%u %u\n", A.row_dim (), A.col_dim (), A.coldim (), A.rowdim ());
#endif

	fwrite(buffer, sizeof(char), strlen(buffer), outStream);


	for(int row=0; row<A.row_dim(); ++row)
	{
		int col;
		//for each bit in the mpz_t row
		for(col=0; col<A.col_dim(); ++col)
		{
			if(mpz_tstbit(A[row], col) == 1)
			{
				output_byte |= (1 << (7 - (col%8)));
			}
			else
			{
				output_byte &= ~(1 << (7 - (col%8)));
			}

			if(col%8 == 7) //flush byte every 8 cols
			{
				fwrite(&output_byte, sizeof(unsigned char), 1, outStream);
				output_byte = 0;
			}
		}

		if(col%8 != 0)
			fwrite(&output_byte, sizeof(unsigned char), 1, outStream);

		fflush(outStream);
	}

	fclose(outStream);
}
}