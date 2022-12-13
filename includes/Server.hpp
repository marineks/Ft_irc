#ifndef SERVER_HPP
#define	SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"

class Server
{
	private:
		struct addrinfo				_hints;
		struct addrinfo				*_servinfo;
		int							_server_socket_fd;
		std::map<const int, Client>	_clients;
		std::string					_mdp = "pantoufle";
	
	public:
		Server();
		~Server();
		void		setHints();
		std::string	getMdp();

		int			fillServinfo(char *port);
		int			launchServer();
		int			manageServerLoop();
		void		addClient(int client_socket, std::vector<pollfd> &poll_fds);
		void		delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator &it);

};

#endif