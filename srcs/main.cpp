#include "Server.hpp"

int main (void)
{
	Server server;

	char port[5] = "6667"; // Port was hardcoded for now
	
	// The three following functions calls are just set up
	server.setHints();
	server.fillServinfo(port);
	server.launchServer();

	// Below, the main loop for server/client connection
	try
	{
		if (server.manageServerLoop(&server) == FAILURE)
			throw;
	}
	catch(const std::exception& e) { std::cerr << e.what() << '\n'; }

	return (SUCCESS);
}