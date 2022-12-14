#include "Server.hpp"

Server::Server() : _servinfo(NULL), _server_socket_fd(0)
{
	std::cout << YELLOW << "Server Constructor" << RESET << std::endl;
	memset(&_hints, 0, sizeof(_hints));
}

Server::~Server()
{
	std::cout << YELLOW << "Server destructor" << RESET << std::endl;
}

/**
 * @brief Attributes the correct parameters to the structure Hints.
 * 
 */
void	Server::setHints()
{
	_hints.ai_family = AF_INET; // We choose Ipv4
	_hints.ai_socktype = SOCK_STREAM; // We choose to work with TCP stream sockets
	_hints.ai_flags = AI_PASSIVE; // We'll be on localhost by default
}

std::string	Server::getMdp() 
{ 
	return (_mdp);
}

/**
 * @brief Helps set up the structs 'hints' and 'servinfo' of our Server class
 * 
 * @param port Value given by the user (e.g. "6667")
 * @return int Returns SUCCESS or FAILURE whether getaddrinfo works or not
 */
int		Server::fillServinfo(char *port)
{
	if (getaddrinfo(NULL, port, &_hints, &_servinfo) < 0)
	{
		std::cerr << RED << "Flop du addrinfo" << RESET << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);
}

/**
 * @brief This function follows step by step the required function calls to launch the server:
 * 
 * 		1) socket() => get the server socket file descriptor
 * 		2) setsocktop() => enable the configuration of said socket (here, we wanted
 * 							to allow the re-use of a port if the IP address is different)
 * 		3) bind() => Associate the socket with a specific port (here, the one given by the user)
 * 		4) listen() => Wait for any incoming connections to our server socket
 * 
 * @return int 0 for SUCCESS and -1 for FAILURE
 */
int		Server::launchServer()
{
	_server_socket_fd = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
	if (_server_socket_fd == FAILURE)
	{
		std::cerr << RED << "Flop de la socket :(" << RESET << std::endl;
		return (FAILURE);
	}
	int optvalue = 1; // enables the re-use of a port if the IP address is different
	if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) == FAILURE)
	{
		std::cerr << RED << "Impossible to reuse" << RESET << std::endl;
		return (FAILURE);
	}
	if (bind(_server_socket_fd, _servinfo->ai_addr, _servinfo->ai_addrlen) == FAILURE)
	{
		std::cerr << RED << "Bind failed" << RESET << std::endl;;
		return (FAILURE);
	}
	if (listen(_server_socket_fd, BACKLOG) == FAILURE)
	{
		std::cerr << RED << "Listen failed" << RESET << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);
}

void	Server::addClient(int client_socket, std::vector<pollfd> &poll_fds)
{
	pollfd	client_pollfd;
	Client	new_client(client_socket);

	client_pollfd.fd = client_socket;
	client_pollfd.events = POLLIN;
	poll_fds.push_back(client_pollfd);
	
	_clients.insert(std::pair<int, Client>(client_socket, new_client));		// insert a new nod in client map with the fd as key
	std::cout << PURPLE << "ADDED CLIENT SUCCESSFULLY" << RESET << std::endl;
}

void	Server::delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator &it)
{
	std::cout << "je suis dans le del\n";
	std::cout << "Deconnection of client : " << it->fd << std::endl;
	int key = it->fd;
	std::vector<pollfd>::iterator		iterator;
	for (iterator = poll_fds.begin(); iterator != poll_fds.end(); iterator++)
	{
		if (iterator->fd == it->fd)
		{
			close(it->fd);
			poll_fds.erase(iterator);
			_clients.erase(key);
			break;
		}
	}
	std::cout << CYAN << "Client deleted \nTotal Client is now: " << (unsigned int)(poll_fds.size() - 1) << RESET << std::endl;
}

static void	splitMessage(std::vector<std::string> &cmds, std::string msg)
{
	int 		pos = 0;
	std::string	delimiter = "\n";
	std::string	substr;
	
	while ((pos = msg.find(delimiter)) != static_cast<int>(std::string::npos))
	{
		substr = msg.substr(0, pos);
		cmds.push_back(substr);
		msg.erase(0, pos + delimiter.length());
	}
}

void	Server::parseMessage(int const client_fd, std::string message)
{
	std::vector<std::string> cmds;

	splitMessage(cmds, message);
	for (size_t i = 0; i != cmds.size(); i++)
		execCommand(client_fd, cmds[i]);
}

static int	parseCommand(std::string cmd_line, cmd_struct &cmd_infos)
{
	return (SUCCESS);
}


void Server::execCommand(int const client_fd, std::string cmd_line)
{
	std::cout << "cmd line : " << cmd_line << std::endl;
	
	int			i = 0;
	cmd_struct	cmd_infos;
	std::string	validCmds[2] = {"NICK", 
								"USER"};
	
	// parseCommand(cmd_line, cmd_infos);
	
	// while (i < 2 && validCmds[i] != cmd_infos.name)
	// 	i++;

	// switch (i) 
	// {
	// 	case 0: nick(); break;
	// 	case 1: user(); break;
	// 	default: std::cout << "Not a valid cmd" << std::endl;
	// }

}
