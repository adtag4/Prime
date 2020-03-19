#include "pfp/pfp.h"


namespace pfp
{

std::ostream& operator << (std::ostream& out, pfp::ALG algorithm)
{
	switch(algorithm)
	{
		case pfp::ALG::PR:
			out << "P";
			break;
		case pfp::ALG::ECM:
			out << "E";
			break;
		case pfp::ALG::QS:
			out << "Q";
			break;
		default:
			out << "YOU SUCK BECAUSE CODE SHOULD NOT GET HERE >:-)";
	}
	return out;
}

/*************************************
 *        WorkOrder Functions        *
 *************************************/

// Constructor
WorkOrder::WorkOrder(pfp::ALG algorithm, std::string encodedState)
	: algorithm_(algorithm),
	  encodedState_(encodedState)
{
	
}

// retrieval functions
pfp::ALG WorkOrder::getAlgorithm()
{
	return algorithm_;
}

std::string WorkOrder::getEncodedState()
{
	return encodedState_;
}


// encoding/decoding

void WorkOrder::encode(std::ostream& out)
{
	// encode algorithm
	out << algorithm_;

	// separator between header and package
	out << ".";
	
	// state is already encoded for that algorithm, just pipe it along
	out << encodedState_;

	out << "."; // final delimeter (used for decoding)
}

void WorkOrder::decode(std::istream& in)
{
	std::string alg;
	std::getline(in, alg, '.');
	std::getline(in, encodedState_, '.');
	switch(alg.c_str()[0])
	{
		case 'P':
			algorithm_ = pfp::ALG::PR;
			break;
		case 'E':
			algorithm_ = pfp::ALG::ECM;
			break;
		case 'Q':
			algorithm_ = pfp::ALG::QS;
			break;
		default:
			std::cerr << "Broken message, can't do anything" << std::endl;
			algorithm_ = NONE;
	}
}


// friend functions
std::ostream& operator << (std::ostream& out, WorkOrder& data)
{
	data.encode(out);
	return out;
}

std::istream& operator >> (std::istream& in, WorkOrder& data)
{
	data.decode(in);
	return in;
}


/*********************************
 *     WorkResponse Functions    *
 *********************************/

// constructor
WorkResponse::WorkResponse(pfp::ALG algorithm, std::string encodedStart, std::string encodedEnd)
	: algorithm_(algorithm),
	  encodedStart_(encodedStart),
	  encodedEnd_(encodedEnd)
{

}

// getters
pfp::ALG WorkResponse::getAlgorithm()
{
	return algorithm_;
}

std::string WorkResponse::getEncodedStart()
{
	return encodedStart_;
}

std::string WorkResponse::getEncodedEnd()
{
	return encodedEnd_;
}


// encoding/decoding
void WorkResponse::encode(std::ostream& out)
{
	out << algorithm_ << '.';
	out << encodedStart_ << '.';
	out << encodedEnd_ << '.';
}

void WorkResponse::decode(std::istream& in)
{
	std::string alg;
	std::getline(in, alg, '.');
	std::getline(in, encodedStart_, '.');
	std::getline(in, encodedEnd_, '.');

	switch(alg.c_str()[0])
	{
		case 'P':
			algorithm_ = pfp::ALG::PR;
			break;
		case 'E':
			algorithm_ = pfp::ALG::ECM;
			break;
		case 'Q':
			algorithm_ = pfp::ALG::QS;
			break;
		default:
			std::cerr << "Broken message, can't do anything" << std::endl;
			algorithm_ = NONE;
	}

}

// friend functions

std::ostream& operator << (std::ostream& out, WorkResponse& data)
{
	data.encode(out);
	return out;
}

std::istream& operator >> (std::istream& in, WorkResponse& data)
{
	data.decode(in);
	return in;
}

}
