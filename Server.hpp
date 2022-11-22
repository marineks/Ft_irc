#ifndef SERVER_HPP
#define	SERVER_HPP

#include "Irc.hpp"
class Server
{
	private:
		struct addrinfo		_hints;
		struct addrinfo		*_servinfo;
		// struct addrinfo	*_res;
		std::vector<int>	_clientsFds;
		int					_socketFd;
	public:
		Server(/* args */);
		~Server();
		void	setHints(void);
		int		fillServinfo(char *port);
		int		launchServer();
};

#endif