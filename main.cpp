#include "Server.hpp"

int main (void)
{
	Server server;

	char port[5] = "6667";
	server.setHints();
	server.fillServinfo(port);
	server.launchServer();
	try
	{
		std::cout << PURPLE << "Je suis dans le try" << RESET << std::endl;
		if (server.manageServerLoop() == FAILURE)
			throw;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (SUCCESS);
}