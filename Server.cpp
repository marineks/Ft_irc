#include "Server.hpp"

Server::Server() : _servinfo(NULL), _serverSocketFd(0)
{
	std::cout << "Server Constructor\n";
	memset(&_hints, 0, sizeof(_hints));
}

Server::~Server()
{
	std::cout << "Server destructor\n";
}

void	Server::setHints()
{
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE; // on sera sur le localhost par default
}

int		Server::fillServinfo(char *port)
{
	if (getaddrinfo(NULL, port, &_hints, &_servinfo) < 0)
	{
		std::cerr << "Flop du addrinfo\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

int		Server::launchServer()
{
	_serverSocketFd = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
	if (_serverSocketFd == FAILURE)
	{
		std::cerr << "Flop de la socket :(\n";
		return (FAILURE);
	}
	int optvalue = 1; // enables the re-use of a port if the IP address is different
	if (setsockopt(_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) == FAILURE)
	{
		std::cerr << "Impossible to reuse\n";
		return (FAILURE);
	}
	if (bind(_serverSocketFd, _servinfo->ai_addr, _servinfo->ai_addrlen) == FAILURE)
	{
		std::cerr << "Bind failed\n";
		return (FAILURE);
	}
	if (listen(_serverSocketFd, BACKLOG) == FAILURE)
	{
		std::cerr << "listen failed\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

