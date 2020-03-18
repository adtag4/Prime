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

namespace pfp
{

enum class ALG {PR, ECM, QS};

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
		void 		encode(ostream& out);
		void		decode(istream& in);
	protected:
	private:
		pfp::ALG	algorithm_;
		std::string	encodedState_;
		pfp::remoteUser	source_;
}

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

		// called by handleClient() if other is a node
		void handleNode(int nodeSocket, struct sockaddr_in nodeAddr);

		// thread to update network_
		void heartbeat();

		// thread to perform work
		void work();

	private:
		int				listenPort_; // the port this node listens on
		int				listenSocket_; // the fd for the listening tcp socket
		struct sockaddr_in		hint_;
		std::vector<std::thread>	threadPool_;
		bool				shutdown_;
		std::vector<pfp::remoteNode>	network_;
		std::queue<pfp::WorkOrder>	jobs_;
}




}
