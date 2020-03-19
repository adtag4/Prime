#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <queue>

// socket stuffs
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// potentially another to dns resolve

#include "pfp/pfp.h"
#include "algorithm/algorithm.h"

namespace pfp
{

class localNode
{
	public:
		localNode() {};
		~localNode() {};

		localNode(int port);

		void main();

	protected:
		// Sets up socket to listen
		void setupListener();

		// handles a connection
		void handleClient();

		// called by handleClient() if other is a user 
		void handleUser(int userSocket, struct sockaddr_in userAddr);

		// thread to perform work
		void work();

		bool searchForMatch(pfp::WorkOrder& wo, pfp::WorkResponse& wr);

		void corona();

	private:
		int				listenPort_; // the port this node listens on
		int				listenSocket_; // the fd for the listening tcp socket
		struct sockaddr_in		hint_;
		bool				shutdown_;
		std::queue<pfp::WorkOrder>	jobs_;
		int				cycles_;
		std::vector<pfp::WorkResponse>	answers_;
		std::mutex			answer_mutex_;
};




}
