#pragma once 

#include "pfp/pfp.h"

namespace pfp
{

class remoteNode
{
	public:
		remoteNode() {};
		~remoteNode() {};

		remoteNode(std::string IP, int port);

		int makeConnectionTo(); // makes a socket and connects to this node

		void decode(std::istream& in);

		friend std::istream& operator >> (std::istream& in, remoteNode& data);

	protected:
	private:
		std::string	IP_;
		int		port_;
}

std::istream& operator >> (std::istream& in, remoteNode& data);

}
