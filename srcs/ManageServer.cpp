#include "Server.hpp"
#include "Colors.hpp"
#include "Commands.hpp"

int Server::manageServerLoop()
{
	std::vector<pollfd> poll_fds;
	pollfd server_poll_fd;

	server_poll_fd.fd = _server_socket_fd;
	server_poll_fd.events = POLLIN;

	poll_fds.push_back(server_poll_fd);

	while (server_shutdown == false)
	{
		std::vector<pollfd> new_pollfds; // tmp struct hosting potential newly-created fds

		if (poll((pollfd *)&poll_fds[0], (unsigned int)poll_fds.size(), -1) <= SUCCESS) // -1 == no timeout
		{
			if (errno == EINTR)
				break ;
			std::cerr << RED << "[Server] Poll error" << RESET << std::endl;
			throw ;
		}

		std::vector<pollfd>::iterator it = poll_fds.begin();
		while (it != poll_fds.end())
		{
			if (it->revents & POLLIN) // => If the event that occured is a POLLIN (aka "data is ready to recv() on this socket")
			{
				if (it->fd == _server_socket_fd)
				{
					if (this->createClientConnexion(poll_fds, new_pollfds) == CONTINUE)
						continue;
				}
				else // => If the dedicated fd for the Client/Server connection already exists
				{
					if (this->handleExistingConnexion(poll_fds, it) == BREAK)
						break ;
				}
			}
			else if (it->revents & POLLOUT) // = "Alert me when I can send() data to this socket without blocking."
			{
				if (handlePolloutEvent(poll_fds, it, it->fd) == BREAK)
					break;
			}
			else if (it->revents & POLLERR)
			{
				if (handlePollerEvent(poll_fds, it) == BREAK)
					break ;
				else
					return (FAILURE);
			}
			it++;
		}
		poll_fds.insert(poll_fds.end(), new_pollfds.begin(), new_pollfds.end()); // Add the range of NEW_pollfds in poll_fds (helps recalculating poll_fds.end() in the for loop)
	}

	return (SUCCESS);
}