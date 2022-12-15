#include "Client.hpp"

Client::Client(int client_fd) : _client_fd(client_fd)
{
	std::cout << YELLOW << "Client constructor" << RESET << std::endl;
}

Client::~Client()
{
	std::cout << YELLOW << "Client destructor" << RESET << std::endl;
}

int	Client::getClientFd()const
{
	return (_client_fd);
}

void	Client::setNickname(std::string const &nickname)
{
	_nickname = nickname;
	// limiter la size comme sur un irc classique
}

std::string		Client::getNickname()const
{
	return (_nickname);
}

void	Client::setUsername(std::string const &username)
{
	_fullname = username;
}

std::string 	Client::getUsername()const
{
	return (_fullname);
}

void	Client::setRealname(std::string const &realname)
{
	_realname = realname;
}

std::string	Client::getRealname()const
{
	return (_realname);
}

void	Client::printClient()const
{
	std::cout << "Print client" << std::endl;
	std::cout << YELLOW << "fd: " << _client_fd << "\n" \
			<< "nickname: " << _nickname << "\n" \
			<< "Fullname: " << _fullname << "\n"
			<< "Real name: " << _realname << RESET << "\n";

}

// TODO : a compléter
int	Client::is_valid() const
{
	if (_fullname.empty())
		return (FAILURE);
	if (_nickname.empty())
		return (FAILURE);
	if (_realname.empty())
		return (FAILURE);
	return (SUCCESS);
}