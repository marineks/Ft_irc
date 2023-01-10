#include "Server.hpp"

int main (int argc, char **argv)
{
	if (argc == 3)
	{
		Server server(argv[1], argv[2]);

		// char port[5] = "6667"; // Port was hardcoded for now

		// The three following functions calls are just set up
		server.setHints();
		server.fillServinfo(argv[1]);
		server.launchServer();
		std::cout << "The Server password is : " << server.getPassword() << std::endl;
		// Below, the main loop for server/client connection
		try
		{
			if (server.manageServerLoop() == FAILURE)
				throw;
		}
		catch(const std::exception& e) { std::cerr << e.what() << '\n'; }

		return (SUCCESS);
	}
	else
	{
		std::cout << "Correct usage is ./ircserv [port] [password] :)" << std::endl;
		return (FAILURE);
	}
		
	
}