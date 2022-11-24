#include "Server.hpp"

static int	acceptSocket(int listenSocket)
{
	sockaddr_in	client;
	socklen_t	addr_size = sizeof(sockaddr_in);
	return (accept(listenSocket, (sockaddr *)&client, &addr_size));
}

static int	addClient(int client_socket, std::vector<pollfd> pollFds)
{
	pollfd	client_pollfd;
	client_pollfd.fd = client_socket;
	client_pollfd.events = POLLIN;
	pollFds.push_back(client_pollfd);
	// print ?
}

int		Server::manageServerLoop()
{
	pollfd	serverPollFd;
	
	serverPollFd.fd = _serverSocketFd;
	serverPollFd.events = POLLIN;
	std::vector<pollfd>	pollFds;
	pollFds.push_back(serverPollFd);
	while (1)
	{
		if (poll((pollfd *)&pollFds[0], (unsigned int)pollFds.size(), -1) <= SUCCESS) // -1 == no timeout
		{
			std::cerr << "poll error\n";
			return (FAILURE);
		}
		std::vector<pollfd>::iterator	it;
		std::vector<pollfd>::iterator	end = pollFds.end();
		for (it = pollFds.begin(); it != end; it++)
		{
			if (it->revents & POLLIN)
			{
				if (it->fd == _serverSocketFd)
				{
					int	clientSock = acceptSocket(_serverSocketFd);
					if (clientSock == -1)
					{
						std::cerr << "Accept failed\n";
						continue;
					}
					if (pollFds.size() - 1 < MAX_CLIENT_NB)
						addClient(clientSock, pollFds);
				}
			}
		}
	}
}