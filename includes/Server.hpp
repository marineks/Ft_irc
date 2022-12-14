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
		// Constructor & destructor
		Server();
		~Server();
		// Accessors
		void		setHints();
		std::string	getMdp();

		// Running Server functions
		int			fillServinfo(char *port);
		int			launchServer();
		int			manageServerLoop();
		// Manage Clients functions
		void		addClient(int client_socket, std::vector<pollfd> &poll_fds);
		void		delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator &it);
		// Parsing & Commands functions
		void		parseMessage(const int client_fd, std::string message);
		void		execCommand(int const client_fd, std::string cmd_line);

};

#endif