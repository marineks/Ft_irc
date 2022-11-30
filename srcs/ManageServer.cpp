#include "Server.hpp"
#include "Colors.hpp"

static int	acceptSocket(int listenSocket)
{
	sockaddr_in	client;
	socklen_t	addr_size = sizeof(sockaddr_in);
	return (accept(listenSocket, (sockaddr *)&client, &addr_size));
}

static void	addClient(int client_socket, std::vector<pollfd> &poll_fds)
{
	pollfd	client_pollfd;
	client_pollfd.fd = client_socket;
	client_pollfd.events = POLLIN | POLLOUT;
	poll_fds.push_back(client_pollfd);
	std::cout << PURPLE << "ADDED CLIENT SUCCESSFULLY" << RESET << std::endl;
}

static void	tooManyClients(int client_socket)
{
	std::cout << RED << ERR_FULL_SERV << RESET << std::endl;
	send(client_socket, ERR_FULL_SERV, strlen(ERR_FULL_SERV) + 1, 0);
	close(client_socket);
}

static sockaddr_in	getClientAddress(int socket)
{
	sockaddr_in client;
	socklen_t addrSize = sizeof(struct sockaddr_in);
	if (getpeername(socket, (struct sockaddr*)&client, &addrSize) != SUCCESS)
	{
		std::cerr << RED << "Get Client Address Error" << RESET << std::endl;
		throw ;
	}
	return (client);
}

static void	print(std::string type, int client_socket, char *message)
{
	sockaddr_in client = getClientAddress(client_socket);
	std::cout << PURPLE << type \
			  << RESET << client_socket << " " \
			  << inet_ntoa(client.sin_addr) << " " \
			  << ntohs(client.sin_port) << std::endl \
			  << BLUE << (message == NULL ? "\n" : message) << RESET << std::endl;
}

static void	delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator &it)
{
	std::cout << "je suis dans le del\n";
	print("Deconnection of client : ", it->fd, NULL);
	std::vector<pollfd>::iterator		iterator;
	for (iterator = poll_fds.begin(); iterator != poll_fds.end(); iterator++)
	{
		if (iterator->fd == it->fd)
		{
			close(it->fd);
			poll_fds.erase(iterator);
			break;
		}
	}
	// close(it->fd);
	// poll_fds.erase(it);
	std::cout << CYAN << "Client deleted \nTotal Client is now: " << (unsigned int)(poll_fds.size() - 1) << RESET << std::endl;
}

int		Server::manageServerLoop()
{
	pollfd	server_poll_fd;
	
	server_poll_fd.fd = _server_socket_fd;
	server_poll_fd.events = POLLIN;
	std::vector<pollfd>	poll_fds;
	poll_fds.push_back(server_poll_fd);
	while (1)
	{
		std::vector<pollfd> new_pollfds;

		if (poll((pollfd *)&poll_fds[0], (unsigned int)poll_fds.size(), -1) <= SUCCESS) // -1 == no timeout
		{
			std::cerr << RED << "Poll error" << RESET << std::endl;;
			return (FAILURE);
		}
		std::vector<pollfd>::iterator	it;
		for (it = poll_fds.begin(); it != poll_fds.end();)
		{
			// std::cout << GREEN << "it->fd au debut du for :" << it->fd << RESET << std::endl;
			if (it->revents & POLLIN)
			{
				if (it->fd == _server_socket_fd)
				{
					int	client_sock = acceptSocket(_server_socket_fd);
					if (client_sock == -1)
					{
						std::cerr << RED << "Accept failed" << RESET << std::endl;
						continue;
					}
					if (poll_fds.size() - 1 < MAX_CLIENT_NB)
					{
						addClient(client_sock, new_pollfds);
						it++;
					}
					else
						tooManyClients(_server_socket_fd);
				}
				else
				{
					// client echo message
					char	message[BUF_SIZE_MSG];
					int res;

					memset(message, 0, sizeof(message));
					res = recv(it->fd, message, BUF_SIZE_MSG, 0);

					if (res <= FAILURE)
					{
						std::cout << "456 recv failed\n";
						delClient(poll_fds, it);
					}
					else if (res == 0)
					{
						// disconnect
						delClient(poll_fds, it);
						std::cout << "Disconnected\n";
					}
					else
					{
						print("Recv : ", it->fd, message); // si affichage incoherent regarder ici 
						// parsing 
						// send(it->fd, message, strlen(message) + 1, 0);
						// print("Send : ", it->fd, message);
						it++;
					}
				}
			}
			else if (it->revents & POLLERR) // voir si il faut it++ ?
			{
				std::cout << "je suis dans le POLLERR\n";
				if (it->fd == _server_socket_fd)
				{
					std::cerr << RED << "Lister socket error" << RESET << std::endl;
					return (FAILURE);
				}
				else
				{
					std::cout << "dans le else\n";
					delClient(poll_fds, it);
				}
			}
			else if (it->revents & POLLOUT)
			{
				// send(it->fd, ":127.0.0.1 001 tmanolis :Welcome tmanolis!tmanolis@127.0.0.1\r\n", 62, 0);
				// TODO flush buffer in client
				it++;
			}
			else
				it++;
			// std::vector<pollfd>::iterator	it1;
			// std::vector<pollfd>::iterator	end1 = poll_fds.end();
			// for (it1 = poll_fds.begin(); it1 != end1; it1++)
			// 	std::cout << YELLOW << "Fd = " << it1->fd << RESET << std::endl;
		}
		poll_fds.insert(poll_fds.end(), new_pollfds.begin(), new_pollfds.end());
	}
	return (SUCCESS);
}