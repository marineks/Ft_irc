#ifndef SERVER_HPP
#define	SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <iostream>
#include <fstream>
#include <stdlib.h>

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
		std::vector<server_op>			_irc_operators;
	
	public:
		// Constructor & destructor
		Server(std::string port, std::string password);
		Server();
		~Server();

		// Accessors
		void								setHints();
		std::string							getPort() const;
		std::string							getPassword() const;
		void								setPassword(std::string new_pwd);
		std::map<std::string, Channel>& 	getChannels();
		std::map<const int, Client>&		getClients();
		std::vector<server_op>&				getIrcOperators(); 
		
		// Running Server functions
		int 		readFromConfigFile(char *filename);
		int			fillServinfo(char *port);
		int			launchServer();
		int			manageServerLoop();

		// Manage Clients functions
		void		addClient(int client_socket, std::vector<pollfd> &poll_fds);
		void 		delClient(std::vector<pollfd> &poll_fds, int current_fd);
		void 		fillClients(std::map<const int, Client> &client_list, int client_fd, std::string cmd);

		// Parsing & Commands functions
		void		parseMessage(const int client_fd, std::string message);
		void		execCommand(int const client_fd, std::string cmd_line);

		// Display functions
		void		printClientList();
		void		printChannels();
		void		printOper(std::string &channelName);

		// Custom exceptions
		class InvalidClientException : public std::exception {
			public :
					const char *	what (void) const throw();
		};
		// Channels modes
		bool	isChannel(std::string &channelName);
		void	addChannel(std::string &channelName);
		void	addClientToChannel(std::string &channelName, Client &client);
		void	banClientFromChannel(std::string &channelName, std::string client_nickname, std::string operator_nickname);
		void	managePassword(std::string datas[4], char sign, int &client_fd);
		void	manageLimit(std::string datas[4], int &client_fd);
		void	manageSecret(std::string datas[4], int &client_fd);
		void	managePrivate(std::string datas[4], int &client_fd);
		void	manageTopicProtection(std::string datas[4], int &client_fd);
		void	manageOperator(std::string datas[4], int &client_fd);
		bool	is_operator(std::string &channelName, std::string &clientName);
		void	manageVoice(std::string datas[4], int &client_fd);
		void	manageModeration(std::string datas[4], int &client_fd);
	};

#endif