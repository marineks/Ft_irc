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

int	Server::createClientConnexion(std::vector<pollfd>& poll_fds, std::vector<pollfd>& new_pollfds)
{
	int client_sock = acceptSocket(_server_socket_fd); // Accepts the socket and returns a dedicated fd for this new Client-Server connexion
	if (client_sock == -1)
	{
		std::cerr << RED << "[Server] Accept() failed" << RESET << std::endl;
		return (CONTINUE);
	}
	if (poll_fds.size() - 1 < MAX_CLIENT_NB)
		addClient(client_sock, new_pollfds); // Beware, here we push the new client_socket in NEW_pollfds
	else
		tooManyClients(client_sock);
	return (SUCCESS);
}

static void print(std::string type, int client_socket, char *message)
{
	if (message)
		std::cout << std::endl << type << client_socket << " << "\
		 << BLUE << message << RESET;
}

int	Server::handleExistingConnexion(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it)
{
	Client *client;
	client = getClient(this, it->fd);
	char message[BUF_SIZE_MSG];
	int read_count;
	
	memset(message, 0, sizeof(message));
	read_count = recv(it->fd, message, BUF_SIZE_MSG, 0); // Retrieves the Client's message

	if (read_count <= FAILURE) // when recv returns an error
	{
		std::cerr << RED << "[Server] Recv() failed [456]" << RESET << std::endl;
		delClient(poll_fds, it, it->fd);
		return (BREAK);
	}
	else if (read_count == 0) // when a client disconnects
	{
		std::cout << "[Server] A client just disconnected\n";
		delClient(poll_fds, it, it->fd);
		return (BREAK);
	}
	else
	{
		print("[Client] Message received from client ", it->fd, message); // si affichage incoherent regarder ici
		client->setReadBuffer(message);

		if (client->getReadBuffer().find("\r\n") != std::string::npos)
		{
			try 
			{
				parseMessage(it->fd, client->getReadBuffer());
				if (client->getReadBuffer().find("\r\n"))
					client->getReadBuffer().clear();
			}
			catch(const std::exception& e) 
			{ 
				std::cout << "[SERVER] Caught exception : ";
				std::cerr << e.what() << std::endl;
				if (client->isRegistrationDone() == true)
					client->setDeconnexionStatus(true);
				return (BREAK);
			}
		}
	}
	return (SUCCESS);
}

int	Server::handlePolloutEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it, const int current_fd)
{
	Client *client = getClient(this, current_fd);
	if (!client)
		std::cout << "[Server] Did not found connexion to client sorry" << std::endl;
	else
	{
		sendServerRpl(current_fd, client->getSendBuffer());
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

