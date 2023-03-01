#include "Server.hpp"
#include "Commands.hpp"

// Server::Server()
Server::Server(std::string port, std::string password, struct tm *timeinfo)
: _servinfo(NULL), _server_socket_fd(0) , _port(port), _password(password)
{
	std::cout << YELLOW << "Server running..." << RESET << std::endl;
	std::cout << YELLOW << "Server listening" << RESET << std::endl;
	memset(&_hints, 0, sizeof(_hints));
	this->setDatetime(timeinfo);
}

Server::~Server()
{
	std::cout << YELLOW << "Server shutting down..." << RESET << std::endl;
}

const char * 	Server::InvalidClientException::what (void) const throw() 
{
	return "The information given by the client are invalid.";
}

/**
 * @brief Attributes the correct parameters to the structure Hints.
 *
 */
void Server::setHints()
{
	_hints.ai_family = AF_INET;		  // We choose Ipv4
	_hints.ai_socktype = SOCK_STREAM; // We choose to work with TCP stream sockets
	_hints.ai_flags = AI_PASSIVE;	  // We'll be on localhost by default
}

std::string 					Server::getPort()	  const { return (_port); }

std::string 					Server::getPassword() const { return (_password); }

std::string 					Server::getDatetime() const { return (_datetime); }

std::map<std::string, Channel>&	Server::getChannels()		{ return (_channels); }

std::map<const int, Client>&	Server::getClients()		{ return (_clients); }

std::vector<server_op>&			Server::getIrcOperators()	{ return (_irc_operators); }

std::string 					Server::getMotd() const 	{ return (_motd); }

void							Server::setPassword(std::string new_pwd)
{
	_password = new_pwd;
}

void							Server::setDatetime(struct tm *timeinfo)
{
	char buffer[80];

	strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
  	std::string str(buffer);

	_datetime = str;
}

void							Server::setMotd(std::string buffer)
{
	_motd =  buffer;
}

/**
 * @brief Get the name, host and password of all the irc operators from a file
 * 
 * @param file Str of the config file with all of infos
 * @return int Returns SUCCESS (0) or FAILURE (-1)
 */
int 		Server::readFromConfigFile(char *filename)
{
	std::ifstream				data;
	std::string					buffer;
	std::vector<std::string>	operators;

	data.open(filename);
	if (!data)
		return (FAILURE);
	while (getline(data, buffer)) {
		operators.push_back(buffer);
	}
	data.close();

	std::vector<std::string>::iterator it;
	for (it = operators.begin(); it != operators.end(); it++)
	{
		std::string	line = *it;
		server_op	op;
		
		int len = line.size() - (line.size() - line.find_first_of(' '));

		op.name.insert(0, line, 0, len);
		op.host.insert(0, line, len + 1, line.find_last_of(' ') - len - 1);
		op.password.insert(0, line, line.find_last_of(' ') + 1, line.size() - 1);
		
		_irc_operators.push_back(op);
	}
   return (SUCCESS);
}

/**
 * @brief Helps set up the structs 'hints' and 'servinfo' of our Server class
 *
 * @param port Value given by the user (e.g. "6667")
 * @return int Returns SUCCESS or FAILURE whether getaddrinfo works or not
 */
int Server::fillServinfo(char *port)
{
	if (getaddrinfo(NULL, port, &_hints, &_servinfo) < 0)
	{
		std::cerr << RED << "[Server] Flop du addrinfo" << RESET << std::endl;
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
int Server::launchServer()
{
	_server_socket_fd = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
	if (_server_socket_fd == FAILURE)
	{
		std::cerr << RED << "[Server] Flop de la socket :(" << RESET << std::endl;
		return (FAILURE);
	}
	int optvalue = 1; // enables the re-use of a port if the IP address is different
	if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) == FAILURE)
	{
		std::cerr << RED << "[Server] Impossible to reuse" << RESET << std::endl;
		return (FAILURE);
	}
	if (bind(_server_socket_fd, _servinfo->ai_addr, _servinfo->ai_addrlen) == FAILURE)
	{
		std::cerr << RED << "[Server] Bind failed" << RESET << std::endl;
		return (FAILURE);
	}
	if (listen(_server_socket_fd, BACKLOG) == FAILURE)
	{
		std::cerr << RED << "[Server] Listen failed" << RESET << std::endl;
		return (FAILURE);
	}
	freeaddrinfo(_servinfo);
	return (SUCCESS);
}

void Server::addClient(int client_socket, std::vector<pollfd> &poll_fds)
{
	pollfd client_pollfd;
	Client new_client(client_socket);

	client_pollfd.fd = client_socket;
	client_pollfd.events = POLLIN | POLLOUT; 
	poll_fds.push_back(client_pollfd);

	_clients.insert(std::pair<int, Client>(client_socket, new_client)); // insert a new nod in client map with the fd as key
	std::cout << PURPLE << "[Server] Added client #" << client_socket << " successfully" << RESET << std::endl;
}

void Server::delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator &it, int current_fd)
{
	std::cout << "[Server] Deconnection of client #" << current_fd << std::endl;

	int key = current_fd;

	close(current_fd);
	_clients.erase(key);
	poll_fds.erase(it);

	std::cout << "[Server] " << PURPLE << "Client deleted. Total Client is now: " << (unsigned int)(poll_fds.size() - 1) << RESET << std::endl;
}

void Server::addChannel(std::string &channelName)
{
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it != _channels.end())
	{
		std::cout << "Channel already exists, choose an other name\n";
		return ;
	}
	Channel	channel(channelName);
	_channels.insert(std::pair<std::string, Channel>(channel.getName(), channel));
}

void Server::addClientToChannel(std::string &channelName, Client &client)
{
	std::map<std::string, Channel>::iterator it;
	it = _channels.find(channelName);
	std::string client_nickname = client.getNickname();
	if (it->second.doesClientExist(client_nickname) == false)
	{
		it->second.addClientToChannel(client);
		std::cout << "Client successfully joined the channel" << channelName << "!" << std::endl;
	}
	else 
		std::cout << YELLOW << client.getNickname() << "already here\n" << RESET;
}

bool    Server::isChannel(std::string &channelName)
{
    std::map<std::string, Channel>::iterator it;
    it = _channels.find(channelName);
    if (it == _channels.end())
    {
        std::cout << RED << "This channel does not exists\n" << RESET;
        return (false);
    }
    return (true);
}