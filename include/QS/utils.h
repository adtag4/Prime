/*
 * utils.h
 *
 *  Created on: Jan 5, 2013
 *      Author: martani
 *      Copyright martani 2013
 */
#pragma once

#ifndef UTILS_H_
#define UTILS_H_

#include <sys/time.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "QS/debug-cout.h"
#include "QS/linear-algebra/matrix.h"


#ifndef	ENABLE_TIMERS
#define TIMER_DECLARE(s)  		\
	timeval __timer_q_##s; 		\
	timeval __timer_q_end_##s;	\
	double  __timer_q_diff_##s = 0.0;

#define TIMER_START(s)    gettimeofday(&__timer_q_##s, NULL);

#define TIMER_STOP(s) 	  							\
	gettimeofday(&__timer_q_end_##s, NULL);			\
	__timer_q_diff_##s += (__timer_q_end_##s.tv_sec  - __timer_q_##s.tv_sec) * 1000.0;	\
	__timer_q_diff_##s += (__timer_q_end_##s.tv_usec - __timer_q_##s.tv_usec) / 1000.0;

#define TIMER_REPORT(s)			\
	std::cout << "Total [" #s "] time: " << __timer_q_diff_##s << " ms."	\
			  << std::endl;
#else
#define TIMER_DECLARE(s)
#define TIMER_START(s)
#define TIMER_STOP(s)
#define TIMER_REPORT(s)
#endif

namespace Utils {

/*
 * Appends a tab \t separated values of the vector v to a file.
 * Inserts a new line (\t\n) at the end.
 */
template<typename T>
static void AppendVectorToFile(std::vector<T>& v, std::string file_name)
{
	std::ofstream file;
	file.open (file_name.c_str (), std::ios::app);

	for(int i=0; i<v.size (); ++i)
	{
		file << v[i] << "\t";
	}
	file << "\n";

	file.close ();
}


static void dumpMatrixAsPbmImage(const Matrix& A, std::string output_file_name);



}

#endif /* UTILS_H_ */
