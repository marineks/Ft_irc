#include "Irc.hpp"
#include "Server.hpp"
#include "Commands.hpp"

void	sendServerRpl(int const client_fd, std::string reply)
{
	send(client_fd, reply.c_str(),reply.size(), 0);
	std::cout << "[Server] Message sent to client " \
			<< client_fd << " >> " << CYAN << reply << RESET << std::endl;
}