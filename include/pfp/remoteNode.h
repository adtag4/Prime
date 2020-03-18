

namespace pfp
{

class remoteNode
{
	public:
		remoteNode() {};
		~remoteNode() {};

		remoteNode(std::string IP, int port);

		int makeConnectionTo(); // makes a socket and connects to this node
	protected:
	private:
		std::string	IP_;
		int		port_;
}

}
