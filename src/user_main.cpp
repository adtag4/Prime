#include "pfp/localUser.h"

int main(int argc, char **argv)
{
	// TODO: argument for port

	pfp::localUser usr("nodes.txt");
	usr.main();
}
