// quadratic.h


/*********************************************************
 *                 DOCUMENTATION NOTICE                  *
 *   The quadratic sieve algorithm is very complicated   *
 *  and a nice implementation in C++ already exists on   *
 *  github at github.com/martani/Quadratic-Sieve-Cxx     *
 *  which we have elected to utilize for performing the  *
 *  computation.  All files under the QS directories are *
 *  taken from that project with minor modification.     *
 *********************************************************/


#pragma once

#include "algorithm/algorithm.h"
#include "QS/QS.h"

namespace alg
{

class QuadSieveState : public AlgStateData
{
	public:
		QuadSieveState() {}; // DONT USE
		~QuadSieveState() {};
		
		QuadSieveState(alg::INT n, int nbLinRels)
			: n_(n),
			  nbLinRels_(nbLinRels),
			  factor_("0")
		{};

		alg::INT	n_;
		int		nbLinRels_;
		alg::INT	factor_;
	protected:
		void encode(std::ostream& out) override;
		void decode(std::istream& in)  override;

};

class QuadSieve : public Algorithm
{
	public:
		QuadSieve() : current_(), qsMachine_(alg::INT("2019"), 5) {}; // DONT USE
		~QuadSieve() {};
		
		QuadSieve(QuadSieveState& startState) 
			: current_(startState),
			  qsMachine_(current_.n_, current_.nbLinRels_)
		{
			// creates qs evaluation machine
			// NOTE: THE "QS" CLASS IS FROM github.com/martani/Quadratic-Sieve-Cxx

			// since evaluation is single-shot, not progressive, start here
			qsMachine_.Factor();
		};

		void proceed() override {}; // all is done in QS when this class is constructed

		bool foundFactor() override;

		AlgStateData& currentState() override;

		
		

	private:
		QuadSieveState	current_;
		QS		qsMachine_;
		
};



}

