#include "pfp/localNode.h"

int main(int argc, char **argv)
{
	// TODO: argument for port

	pfp::localNode node(10000);
	node.main();
}
