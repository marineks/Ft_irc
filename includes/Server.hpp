#ifndef SERVER_HPP
#define	SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <iostream>
#include <fstream>
#include <csignal>

extern bool	server_shutdown;

struct server_op
{
	std::string name;
	std::string	host;
	std::string	password;
};

class Server
{
	private:
		struct addrinfo					_hints;
		struct addrinfo					*_servinfo;
		int								_server_socket_fd;
		std::map<const int, Client>		_clients;
		std::map<std::string, Channel>	_channels;
		std::string						_port;
		std::string						_password;
		std::string						_datetime;
		std::vector<server_op>			_irc_operators;
		std::string						_motd;
	
	public:
		// Constructor & destructor
		Server(std::string port, std::string password, struct tm *timeinfo);
		Server();
		~Server();
		// Accessors
		void								setHints();
		std::string							getPort() const;
		std::string							getPassword() const;
		void								setPassword(std::string new_pwd);
		std::string							getDatetime() const;
		void								setDatetime(struct tm *timeinfo);
		std::map<std::string, Channel>& 	getChannels();
		std::map<const int, Client>&		getClients();
		std::vector<server_op>&				getIrcOperators();
		std::string							getMotd() const;
		void								setMotd(std::string buffer);
		
		// Running Server functions
		int 		readFromConfigFile(char *filename);
		int			fillServinfo(char *port);
		int			launchServer();
		int			manageServerLoop();
		int			createClientConnexion(std::vector<pollfd>& poll_fds, std::vector<pollfd>& new_pollfds);
		int			handleExistingConnexion(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it);
		int			handlePolloutEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it, const int current_fd);
		int			handlePollerEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it);
		// Manage Clients functions
		void		addClient(int client_socket, std::vector<pollfd> &poll_fds);
		// void 		delClient(std::vector<pollfd> &poll_fds, int current_fd);
		void 		delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator &it, int current_fd);
		void 		fillClients(std::map<const int, Client> &client_list, int client_fd, std::string cmd);
		// Parsing & Commands functions
		void		parseMessage(const int client_fd, std::string message);
		void		execCommand(int const client_fd, std::string cmd_line);
		// Display functions
		void		printChannel(std::string &channelName);
		void		printOper(std::string &channelName);
		// Custom exceptions
		class InvalidClientException : public std::exception {
			public :
					const char *	what (void) const throw();
		};
		// Channel functions
		void	addChannel(std::string &channelName);
		void	addClientToChannel(std::string &channelName, Client &client);
		bool	isChannel(std::string &channelName);
};

#endif