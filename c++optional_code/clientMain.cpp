#include "Client.h"
#include <memory>


int main()
{
	std::unique_ptr<Client> c = std::make_unique<Client>();
	c->createSocket();
	c->connected();
	c->shutdown();

	return 0;

}

