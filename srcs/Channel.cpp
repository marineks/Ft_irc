#include "Channel.hpp"

/*
*				################################
*				###	CONSTRUCTOR & DESTRUCTOR ###
*				################################
*/

Channel::Channel(std::string const &channelName): _name(channelName), _limit(-1), _secret(false),\
	_private(false), _topic_protection(false), _moderation(false)
{
	_banned_users.clear();
	_clientList.clear();
	_topic.clear();
	_password.clear();
}

Channel::~Channel() {}

/*
*				################################
*				###			ACCESSORS 		 ###
*				################################
*/

std::string&						Channel::getName() 			{ return (_name); }
std::string&						Channel::getTopic() 		{ return (_topic); }
std::map <std::string, Client>&		Channel::getClientList()	{ return (_clientList); }
std::vector<std::string>&			Channel::getBannedUsers()	{ return (_banned_users); }
std::vector<std::string>&			Channel::getKickedUsers()	{ return (_kicked_users); }
std::vector<std::string>&			Channel::getOperators() 	{ return (_operators); }
int									Channel::getLimit()			{ return (_limit); }
std::string&						Channel::getPassword()		{ return (_password); }
bool								Channel::getSecret()const	{ return (_secret); }
bool								Channel::getPrivate()const	{ return (_private); }
bool								Channel::getModeration()const {return (_moderation); }
bool								Channel::getTopicProtection()const { return (_topic_protection); }

void		Channel::setPassword(std::string &password) { _password = password;}
void		Channel::setLimit(int &value)				{ _limit = value;}
void		Channel::setTopic(std::string& newTopic)	{ _topic = newTopic;}
void		Channel::setSecret(int i)					{_secret = i;}
void		Channel::setPrivate(int i)					{_private = i;}
void		Channel::setTopicProtection(int i)			{_topic_protection = i;}
void		Channel::setModeration(bool i)				{_moderation = i;}

bool		Channel::doesClientExist(std::string &clientName)
{	
	if (_clientList.size() == 0)
		return (false);

	std::map <std::string, Client>::iterator it = _clientList.find(clientName);
	if (it == _clientList.end())
		return (false);
	return (true);
}

/*
*				################################
*				### MANAGE CLIENT FUNCTIONS  ###
*				################################
*/

void	Channel::printClientList()
{
	std::cout <<  "Here is the Client list of the Channel " << YELLOW << this->getName() << RESET << std::endl;
	for (std::map <std::string, Client>::iterator it = _clientList.begin(); \
		 it != _clientList.end(); it++)
		std::cout << it->first << std::endl;
}

/**
 * @brief Removes a Client from the ClientList of the Channel.
 * 		  Also strips this Client of the Operator privileges.
 * 
 * @param clientName 
 */
void	Channel::removeClientFromChannel(std::string &clientName)
{
	std::map <std::string, Client>::iterator it = this->_clientList.find(clientName);
	if (it != _clientList.end())
	{
		this->_clientList.erase(it);
		std::cout << clientName << " has been erased from client list\n";
	}
	
	// This function checks if the ClientName is present in the Operator list
	// and if so, deletes it.
	removeOperator(clientName); 
}

/*
*				################################
*				###	 CLIENT STATUS FUNCTIONS ###
*				################################
*/

int	Channel::addClientToChannel(Client &client)
{
	if ((int)_clientList.size() == _limit)
	{
		std::cout << "Channel reached its maximum capacity\n";
		return (1);
	}
	_clientList.insert(std::pair<std::string, Client>(client.getNickname(), client));
	return (0);
}

void	Channel::addToKicked(std::string &kicked_name)
{
	std::vector<std::string>::iterator it;
	for (it = _kicked_users.begin(); it != _kicked_users.end(); it++)
	{
		if (*it == kicked_name)
		{
			std::cout << kicked_name << " is already kicked from the channel " << getName() << std::endl;
			return ;
		}
	}
	_kicked_users.push_back(kicked_name);
	std::cout << RED << kicked_name << " is now kicked from the channel " << getName() << RESET << std::endl;
}

void	Channel::addToBanned(std::string &banned_name)
{
	std::vector<std::string>::iterator it;
	for (it = _banned_users.begin(); it != _banned_users.end(); it++)
	{
		if (*it == banned_name)
		{
			std::cout << banned_name << " is already banned from the channel " << getName() << std::endl;
			return ;
		}
	}
	_banned_users.push_back(banned_name);
	std::cout << RED << banned_name << " is now banned from the channel " << getName() << RESET << std::endl;
}

void	Channel::removeFromBanned(std::string &banned_name)
{
	std::vector<std::string>::iterator user;
	for (user = _banned_users.begin(); user != _banned_users.end(); user++)
	{
		if (*user == banned_name)
		{
			_banned_users.erase(user);
			std::cout << banned_name << " is not banned anymore from the channel " << getName() << std::endl;
			return ;
		}
	}
	std::cout << "No need! " << banned_name << " has never been banned from the channel " << getName() << std::endl;
}

bool	Channel::isBanned(std::string &banned_name)
{
	std::vector<std::string>::iterator user;
	if (_banned_users.empty() == true) // work
		return (false);
	for (user = _banned_users.begin(); user != _banned_users.end(); user++)
	{
		if (*user == banned_name)
			return (true);
	}
	return (false);	
}

bool	Channel::goodPassword(std::string &password)
{
	if (_password.empty() == false)
		std::cout << "password ? " << _password << std::endl;
	else
	{
		std::cout << "no password required\n";
		return (true);
	}
	if (_password.empty() == false && password != _password)
	{
		std::cout << "Wrong password sorray!\n";
		return (false);
	}
	return (true);
}

/*
*				################################
*				###	  OPERATORS FUNCTIONS    ###
*				################################
*/

void	Channel::addFirstOperator(std::string operatorName)
{
	if (_operators.empty())
		_operators.push_back(operatorName);
}

void	Channel::removeOperator(std::string operatorName)
{
	std::vector<std::string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == operatorName)
			_operators.erase(it);
	}
}

void	Channel::addOperator(std::string operatorName)
{
	std::vector<std::string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == operatorName)
		{
			std::cout << operatorName << "is already an Operator\n";
			return ;
		}
	}
	_operators.push_back(operatorName);
}

bool 	Channel::isOperator(std::string &operatorName)
{
	std::vector<std::string>::iterator user;
	if (_operators.empty())
		return (false);
	for (user = _operators.begin(); user != _operators.end(); user++)
	{
		if (*user == operatorName)
			return (true);
	}
	return (false);
}

void	Channel::printOperators(void)
{
	std::cout << "Print operators : \n";
	std::vector<std::string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}

void	Channel::addVoice(std::string &clientName)
{
	if (this->hasVoice(clientName) == true)
		return ;
	_voiced.push_back(clientName);
}

void	Channel::removeVoice(std::string &clientName)
{
	std::vector<std::string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == clientName)
		{
			_voiced.erase(it);
			return ;
		}
	}
	std::cout << clientName << " never had a voice\n";
}

bool	Channel::hasVoice(std::string &clientName)
{
	std::vector<std::string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == clientName)
		{
			std::cout << clientName << "already has a voice\n";
			return (true);
		}
	}
	return (false);
}