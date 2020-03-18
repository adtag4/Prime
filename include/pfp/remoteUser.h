#pragma once 

#include "pfp/pfp.h"

namespace pfp
{

class remoteUser
{
	public:
		remoteUser() {};
		~remoteUser() {};
		
		remoteUser(std::string IP, int port);
		
		std::string	getIP();
		int			getPort();
		bool		isActive();
		void		setActive(bool val);
	protected:
	private:
		std::string	IP_;
		bool		isActive_;
		int			port_;
}

}