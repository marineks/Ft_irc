#ifndef SERVER_HPP
#define	SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"

class Server
{
	private:
		struct addrinfo		_hints;
		struct addrinfo		*_servinfo;
		// struct addrinfo	*_res;
		std::vector<Client>	_clients;
		int					_serverSocketFd;
	public:
		Server(/* args */);
		~Server();
		void	setHints(void);
		int		fillServinfo(char *port);
		int		launchServer();
		int		manageServerLoop();
};

#endif