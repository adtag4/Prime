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
	protected:
		void encode(ostream& out) override;
		void decode(istream& in)  override;

	private:
		alg::INT	n_;
		int		nbLinRels_;
		alg::INT	factor_;
}

class QuadSieve : public Algorithm
{
	public:
		QuadSieve() {}; // DONT USE
		~QuadSieve() {};
		
		QuadSieve(QuadSieveState& startState) 
			: current_(startState)
		{
			// creates qs evaluation machine
			// NOTE: THE "QS" CLASS IS FROM github.com/martani/Quadratic-Sieve-Cxx
			qsMachine = QS(current_.n_, current_.nbLinRels_);

			// since evaluation is single-shot, not progressive, start here
			qsMachine.Factor();
		};

		void proceed() {} override; // all is done in QS when this class is constructed

		bool foundFactor() override;

		AlgStateData& currentState() override;

		
		

	private:
		QuadSieveState	current_;
		QS		qsMachine_;
		
}



}

