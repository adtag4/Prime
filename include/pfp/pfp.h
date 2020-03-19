#pragma once

#include <string>
#include <iostream>

namespace pfp
{
enum class ALG {PR, ECM, QS, NONE};

std::ostream& operator << (std::ostream& out, pfp::ALG algorithm);

class WorkOrder
{
	public:
		WorkOrder() {};
		~WorkOrder() {};
		
		WorkOrder(pfp::ALG algorithm, std::string encodedState);

		pfp::ALG	getAlgorithm();
		std::string	getEncodedState();
		pfp::remoteUser	getSource();

		void		setSource(pfp::remoteUser source);
		void 		encode(std::ostream& out);
		void		decode(std::istream& in);
		
		friend		std::ostream& operator << (std::ostream& out, WorkOrder& data);
		friend		std::istream& operator >> (std::istream& in,  WorkOrder& data);
	protected:
	private:
		pfp::ALG	algorithm_;
		std::string	encodedState_;
		pfp::remoteUser	source_;
};

class WorkResponse
{
	public:
		WorkResponse() {};
		~WorkResponse() {};
		
		WorkResponse(pfp::ALG algorithm, std::string encodedStart, std::string encodedEnd);
		
		pfp::ALG	getAlgorithm();
		std::string	getEncodedStart();
		std::string	getEncodedEnd();
		
		void		encode(std::ostream& out);
		void		decode(std::istream& in);
		
		friend		std::ostream& operator << (std::ostream& out, WorkResponse& data);
		friend		std::istream& operator >> (std::istream& in,  WorkResponse& data);
	protected:
	private:
		pfp::ALG	algorithm_;
		std::string	encodedStart_;
		std::string	encodedEnd_;
};

std::ostream& operator << (std::ostream& out, WorkResponse& data);
std::istream& operator >> (std::istream& in,  WorkResponse& data);
std::ostream& operator << (std::ostream& out, WorkOrder& data);
std::istream& operator >> (std::istream& in,  WorkOrder& data);

}
