#include "Server.hpp"

static int	acceptSocket(int listenSocket)
{
	sockaddr_in	client;
	socklen_t	addr_size = sizeof(sockaddr_in);
	return (accept(listenSocket, (sockaddr *)&client, &addr_size));
}

static void	addClient(int client_socket, std::vector<pollfd> pollFds)
{
	pollfd	client_pollfd;
	client_pollfd.fd = client_socket;
	client_pollfd.events = POLLIN;
	pollFds.push_back(client_pollfd);
	// print ?
}

static void	tooManyClients(int client_socket)
{
	std::cout << ERR_FULL_SERV << std::endl;
	send(client_socket, ERR_FULL_SERV, strlen(ERR_FULL_SERV) + 1, 0);
	close(client_socket);
}

static sockaddr_in	getClientAddress(int socket)
{
	sockaddr_in client;
	socklen_t addrSize = sizeof(struct sockaddr_in);
	if (getpeername(socket, (struct sockaddr*)&client, &addrSize) != SUCCESS)
	{
		std::cerr << "Get Client Address Error\n";
		throw ;
	}
	return (client);
}

static void	printRecv(std::string type, int client_socket, char *message)
{
	sockaddr_in client = getClientAddress(client_socket);
	std::cout << type \
			  << client_socket << " " \
			  << inet_ntoa(client.sin_addr) << " " \
			  << ntohs(client.sin_port) \
			  << message;
}

static void	delClient(std::vector<pollfd> pollFds, std::vector<pollfd>::iterator it)
{
	pollFds.erase(it);
	// Marine fera le ternaire.
	close(it->fd);
	std::cout << "Client deleted \nTotal Client is now: " << (unsigned int)(pollFds.size() - 1) << std::endl;
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
					else
						tooManyClients(_serverSocketFd);
				}
				else
				{
					// client echo message
					char	message[BUF_SIZE_MSG];
					if (recv(it->fd, message, BUF_SIZE_MSG, 0) <= FAILURE)
						delClient(pollFds, it);
					else
					{
						printRecv("Recv : ", it->fd, message); // si affichage incoherent regarder ici 
						send(it->fd, message, strlen(message) + 1, 0);
						printRecv("Send : ", it->fd, message);
					}
				}
			}
			else if (it->revents & POLLERR)
			{
				if (it->fd == _serverSocketFd)
				{
					std::cerr << "Lister socket error\n";
					return (FAILURE);
				}
				else
					delClient(pollFds, it);
			}
		}
	}
	return (SUCCESS);
}