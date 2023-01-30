#include "Server.hpp"
#include "Colors.hpp"
#include "Commands.hpp"

static int acceptSocket(int listenSocket)
{
	sockaddr_in client;
	socklen_t addr_size = sizeof(sockaddr_in);
	return (accept(listenSocket, (sockaddr *)&client, &addr_size));
}

static void	tooManyClients(int client_socket)
{
	std::cout << RED << ERR_FULL_SERV << RESET << std::endl;
	send(client_socket, ERR_FULL_SERV, strlen(ERR_FULL_SERV) + 1, 0);
	close(client_socket);
}

static void print(std::string type, int client_socket, char *message)
{
	if (message)
		std::cout  << type << client_socket << " << "\
		 << BLUE << message << RESET << std::endl;
}

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
		Client *client;
		while (it != poll_fds.end())
		{
			if (it->revents & POLLIN) // => If the event that occured is a POLLIN (aka "data is ready to recv() on this socket")
			{
				if (it->fd == _server_socket_fd)
				{
					int client_sock = acceptSocket(_server_socket_fd); // Accepts the socket and returns a dedicated fd for this new Client-Server connexion
					if (client_sock == -1)
					{
						std::cerr << RED << "[Server] Accept() failed" << RESET << std::endl;
						continue;
					}
					if (poll_fds.size() - 1 < MAX_CLIENT_NB)
						addClient(client_sock, new_pollfds); // Beware, here we push the new client_socket in NEW_pollfds
					else
						tooManyClients(client_sock);
					it++;
				}
				else // => If the dedicated fd for the Client/Server connection already exists
				{
					client = getClient(this, it->fd);
					char message[BUF_SIZE_MSG];
					int read_count;
					
					memset(message, 0, sizeof(message));
					read_count = recv(it->fd, message, BUF_SIZE_MSG, 0); // Retrieves the Client's message

					if (read_count <= FAILURE) // when recv returns an error
					{
						std::cerr << RED << "[Server] Recv() failed [456]" << RESET << std::endl;
						delClient(poll_fds, it, it->fd);
						break;
					}
					else if (read_count == 0) // when a client disconnects
					{
						std::cout << "[Server] A client just disconnected\n";
						delClient(poll_fds, it, it->fd);
						break ;
					}
					else
					{
						print("[Client] Message received from client ", it->fd, message); // si affichage incoherent regarder ici
						// if (message)
							client->setReadBuffer(message);
						// std::cout << "BUFFER |" << client->getReadBuffer() << "|" << std::endl;
						if (client->getReadBuffer().find("\r\n") != std::string::npos)
						{
							try 
							{
								// std::cout << "JE PARSE LE MESSAGE et le buffer est |" << client->getReadBuffer() << "|" << std::endl;
								parseMessage(it->fd, client->getReadBuffer());
								if (client->getReadBuffer().find("\r\n"))
								{
									client->getReadBuffer().clear();
									// client->resetReadBuffer("");
								}
									
								// std::cout << "JAI FINI et le buffer est |" << client->getReadBuffer() << "|" << std::endl;
							}
							catch(const std::exception& e) 
							{ 
								std::cout << "[SERVER] Caught exception : ";
								std::cerr << e.what() << std::endl;
								if (client->isRegistrationDone() == true)
									client->setDeconnexionStatus(true);
								break ;
							}
						}
						// std::cout << " APRES parsemsg, le buffer est |" << client->getReadBuffer() << "|" << std::endl;
						it++;
					}
				}
			}
			else if (it->revents & POLLOUT) // = "Alert me when I can send() data to this socket without blocking."
			{
				if (handlePolloutEvent(poll_fds, it, it->fd) == BREAK)
					break;
				it++;
			}
			else if (it->revents & POLLERR)
			{
				if (handlePollerEvent(poll_fds, it) == BREAK)
					break ;
				else
					return (FAILURE);
			}
			else
				it++;
		}
		poll_fds.insert(poll_fds.end(), new_pollfds.begin(), new_pollfds.end()); // Add the range of NEW_pollfds in poll_fds (helps recalculating poll_fds.end() in the for loop)
	}

	return (SUCCESS);
}