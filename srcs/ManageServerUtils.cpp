#include "Server.hpp"
#include "Colors.hpp"
#include "Commands.hpp"

int	Server::handlePolloutEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it, const int current_fd)
{
	Client *client = getClient(this, current_fd);
	if (!client)
		std::cout << "[Server] Did not found connexion to client sorry" << std::endl;
	else
	{
		sendServerRpl(current_fd, client->getSendBuffer());
		if (client->getReadBuffer().find("\r\n"))
			client->getReadBuffer().clear();
		client->getSendBuffer().clear();
		if (client->getDeconnexionStatus() == true)
		{
			delClient(poll_fds, it, current_fd);
			return (BREAK);
		}
	}
	return (SUCCESS);
}

int	Server::handlePollerEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it)
{
	if (it->fd == _server_socket_fd)
	{
		std::cerr << RED << "[Server] Listen socket error" << RESET << std::endl;
		return (FAILURE);
	}
	else
	{
		delClient(poll_fds, it, it->fd);
		return (BREAK);
	}
}

