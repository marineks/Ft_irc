#ifndef SERVER_HPP
#define	SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"

class Server
{
	private:
		struct addrinfo		_hints;
		struct addrinfo		*_servinfo;
		std::vector<Client>	_clients;
		int					_server_socket_fd;
	public:
		Server();
		~Server();
		void	setHints();
		int		fillServinfo(char *port);
		int		launchServer();
		int		manageServerLoop();
};

#endif