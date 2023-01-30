#include "Client.hpp"

/*
*				################################
*				###	CONSTRUCTOR & DESTRUCTOR ###
*				################################
*/
Client::Client(int client_fd)
: _client_fd(client_fd), _to_deconnect(false), _connexion_password(false),\
 _registrationDone(false), _welcomeSent(false), _hasAllInfo(false), _nbInfo(0)
{
	// std::cout << YELLOW << "Client constructor for Client #" << client_fd << RESET << std::endl;
}

Client::~Client() {}

/*
*					#################
*					###	ACCESSORS ###
*					#################
*/
int				Client::getClientFd() const { return (_client_fd); }
std::string&	Client::getSendBuffer()  	{ return (_sendbuf); }
std::string&	Client::getReadBuffer()  	{ return (_readbuf); }
std::string&	Client::getNickname()  		{ return (_nickname); }
std::string&	Client::getOldNickname()  	{ return (_old_nickname); }
std::string 	Client::getUsername() const { return (_username); }
std::string		Client::getRealname() const { return (_realname); }
std::string&	Client::getMode()			{ return (_mode); }

bool&			Client::getConnexionPassword()	{ return (_connexion_password); }
bool&			Client::isRegistrationDone() 	{ return (_registrationDone); }
bool&			Client::isWelcomeSent()			{ return (_welcomeSent); }
bool&			Client::hasAllInfo() 			{ return (_hasAllInfo); }
bool&			Client::getDeconnexionStatus()	{ return (_to_deconnect); }
int				Client::getNbInfo() const 		{ return (_nbInfo); }

void	Client::setReadBuffer(std::string const &buf)
{
	_readbuf += buf;
}

void	Client::resetReadBuffer(std::string const &str)
{
	_readbuf.clear();
	_readbuf = str;
}

void	Client::setSendBuffer(std::string const &buf)
{
	_sendbuf += buf;
}

void	Client::setDeconnexionStatus(bool status)
{
	_to_deconnect = status;
}

void	Client::setNickname(std::string const &nickname)
{
	// If the nickname has more than 9 characters, it must be truncated
	_nickname = (_nickname.size() > 9) ? nickname.substr(0, 9) : nickname;
}

void	Client::setOldNickname(std::string const &nickname)
{
	_old_nickname = nickname;
}

void	Client::setUsername(std::string const &username)
{
	_username = username;
}

void	Client::setRealname(std::string const &realname)
{
	_realname = realname;
}

void	Client::addMode(std::string const mode)
{
	if (_mode.empty() == true)
		_mode = "+" + mode;
	else 
		_mode += mode;
}

void	Client::removeMode(std::string const mode)
{
	size_t pos = _mode.find(mode);
	_mode.erase(pos, 1);
}

void	Client::setConnexionPassword(bool boolean)
{
	_connexion_password = boolean;
}

void	Client::setRegistrationDone(bool boolean)
{
	_registrationDone = boolean;
}

void	Client::setWelcomeSent(bool boolean)
{
	_welcomeSent = boolean;
}

void	Client::sethasAllInfo(bool boolean)
{
	_hasAllInfo = boolean;
}

void	Client::setNbInfo(int n)
{
	_nbInfo += n;
	if (_nbInfo <= 0)
		_nbInfo = 0;
}

/*
*					##############################
*					### OTHER MEMBER FUNCTIONS ###
*					##############################
*/

int	Client::is_valid() const
{
	if (_username.empty())
		return (FAILURE);
	if (_nickname.empty())
		return (FAILURE);
	if (_realname.empty())
		return (FAILURE);
	if (_connexion_password == false)
		return (FAILURE);
	if (this->getNbInfo() < 3)
		return (FAILURE);
	return (SUCCESS);
}