#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

std::string	retrieveNickname(std::string msg_to_parse);
static bool	containsInvalidCharacters(std::string nickname);

/**
 * @brief The NICK command is used to give the client a nickname or 
 * 		change the previous one.
 * 
 * 	Syntax: NICK <nickname>
 * 
 * 	Nicknames are non-empty strings with the following restrictions:
 * 	
 * 	They MUST NOT contain any of the following characters: 
 * 	- space (' '), 
 * 	- comma (','), 
 * 	- asterisk ('*'), 
 * 	- question mark ('?'), 
 * 	- exclamation mark ('!'), 
 * 	- at sign ('@'),
 * 	- dot ('.').
 * 	
 * 	They MUST NOT start with any of the following characters: 
 * 	dollar ('$'), colon (':'), diese (#).
 * 	
 * 	Numeric Replies:
 * 
 * 	ERR_NONICKNAMEGIVEN (431)
 * 	ERR_ERRONEUSNICKNAME (432)
 * 	ERR_NICKNAMEINUSE (433)
 * 	ERR_NICKCOLLISION (436)
 * 
 * 	Example:
 * 	[CLIENT] /Nick mike
 * 
 */
void	nick(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	std::string nickname	= retrieveNickname(cmd_infos.message);
	Client&		client		= retrieveClient(server, client_fd);

	if (client.isRegistrationDone() == false)
	{
		client.setNickname(nickname);
		client.setOldNickname(nickname);
		client.setNbInfo(+1);
	}

	if (nickname.empty()) {
		addToClientBuffer(server, client_fd, ERR_NONICKNAMEGIVEN(client.getNickname()));
		if (client.isRegistrationDone() == false)
			client.setNbInfo(-1);
	} 
	else if (containsInvalidCharacters(nickname)) {
		addToClientBuffer(server, client_fd,  ERR_ERRONEUSNICKNAME(client.getNickname(), nickname));
		if (client.isRegistrationDone() == false)
			client.setNbInfo(-1);
	} 
	else if (isAlreadyUsed(server, client_fd, nickname) == true) {
			addToClientBuffer(server, client_fd, ERR_NICKNAMEINUSE(client.getNickname(), nickname));
		if (client.isRegistrationDone() == false)
			client.setNbInfo(-1);
	} else {
		
		if (client.isRegistrationDone() == true)
		{
			client.setOldNickname(client.getNickname());
			std::cout << "[Server] Nickname change registered. Old nickname is now : " << client.getOldNickname() << std::endl;
		
			client.setNickname(nickname);
			
		}
	}
	addToClientBuffer(server, client_fd, RPL_NICK(client.getOldNickname(), client.getUsername(), client.getNickname()));
}

std::string	retrieveNickname(std::string msg_to_parse)
{
	std::string nickname;

	nickname.clear();
	if (msg_to_parse.empty())
		return (nickname);
	if (msg_to_parse.empty() == false && msg_to_parse[0] == ' ')
		msg_to_parse.erase(0, 1);
	if (msg_to_parse.empty() == false && msg_to_parse.find(' '))
	{
		char *str = const_cast<char *>(msg_to_parse.data());
		nickname = strtok(str, " ");
	}
	if (nickname.empty())
		nickname.clear();
	return (nickname);
}

static bool	isForbidden(char c)
{
	if (c == ' ' || c == ',' || c == '*' || c == '?' || c == '!'
		|| c == '@' || c == '.')
		return (true);
	else
		return (false);
}

static bool	containsInvalidCharacters(std::string nickname)
{
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#')
		return (true);
	
	for (size_t i = 0; i < nickname.size(); i++) {
			if (isForbidden(nickname[i]) == true)
				return (true);
	}
	return (false);			
}

bool	isAlreadyUsed(Server *server, int client_fd, std::string new_nickname)
{
	std::map<const int, Client>&			client_list	= server->getClients();
	std::map<const int, Client>::iterator	client		= client_list.begin();

	while (client != client_list.end())
	{
		if (client->second.getClientFd() != client_fd \
			&& client->second.getNickname() == new_nickname)
			return (true);
		client++;
	}
	return (false);
}