#include "Channel.hpp"

/*
*				################################
*				###	CONSTRUCTOR & DESTRUCTOR ###
*				################################
*/

Channel::Channel(std::string const &channelName): _name(channelName), _capacity_limit(-1)
{
	_banned_users.clear();
	_clientList.clear();
	_topic.clear();
}

Channel::~Channel() {}

/*
*				################################
*				###			ACCESSORS 		 ###
*				################################
*/

std::string&						Channel::getName() 			{ return (_name); }
std::string&						Channel::getTopic() 		{ return (_topic); }
std::string&						Channel::getMode()			{ return (_mode); }
std::string&						Channel::getChannelPassword()	{ return (_channel_password); }
int&								Channel::getCapacityLimit()	{ return (_capacity_limit); }
std::map <std::string, Client>&		Channel::getClientList()	{ return (_clientList); }
std::vector<std::string>&			Channel::getBannedUsers()	{ return (_banned_users); }
std::vector<std::string>&			Channel::getKickedUsers()	{ return (_kicked_users); }
std::vector<std::string>&			Channel::getVoicedUsers()	{ return (_voiced_users); }
std::vector<std::string>&			Channel::getOperators() 	{ return (_operators); }

void		Channel::setTopic(std::string& newTopic)
{
	_topic = newTopic;
}

void		Channel::setChannelPassword(std::string password)
{
	_channel_password = password;
}

void		Channel::setCapacityLimit(int limit)
{
	_capacity_limit = limit;
}

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
		this->_clientList.erase(it);
	
	// This function checks if the ClientName is present in the Operator list
	// and if so, deletes it.
	removeOperator(clientName); 
}

/*
*				################################
*				###	 CLIENT STATUS FUNCTIONS ###
*				################################
*/

void	Channel::addClientToChannel(Client &client)
{
	_clientList.insert(std::pair<std::string, Client>(client.getNickname(), client));
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

void	Channel::addToVoiced(std::string &voiced_name)
{
	std::vector<std::string>::iterator it;
	for (it = _voiced_users.begin(); it != _voiced_users.end(); it++)
	{
		if (*it == voiced_name)
		{
			std::cout << voiced_name << " is already voiced from the channel " << getName() << std::endl;
			return ;
		}
	}
	_voiced_users.push_back(voiced_name);
	std::cout << RED << voiced_name << " is now voiced from the channel " << getName() << RESET << std::endl;
}

void	Channel::removeFromVoiced(std::string &voiced_name)
{
	std::vector<std::string>::iterator user;
	for (user = _voiced_users.begin(); user != _voiced_users.end(); user++)
	{
		if (*user == voiced_name)
		{
			_voiced_users.erase(user);
			std::cout << voiced_name << " is not voiced anymore from the channel " << getName() << std::endl;
			return ;
		}
	}
	std::cout << "No need! " << voiced_name << " has never been voiced from the channel " << getName() << std::endl;
}

bool	Channel::isVoiced(std::string &voiced_name)
{
	std::vector<std::string>::iterator user;
	if (_voiced_users.empty() == true) // work
		return (false);
	for (user = _voiced_users.begin(); user != _voiced_users.end(); user++)
	{
		if (*user == voiced_name)
			return (true);
	}
	return (false);	
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

/*
*				################################
*				###	    MODES FUNCTIONS      ###
*				################################
*/

void	Channel::addMode(std::string const mode)
{
	if (_mode.empty() == true)
		_mode = "+" + mode;
	else 
		_mode += mode;
}

void	Channel::removeMode(std::string const mode)
{
	size_t pos = _mode.find(mode);
	_mode.erase(pos, 1);
}

void	Channel::removeChannelPassword()
{
	_channel_password.clear();
}