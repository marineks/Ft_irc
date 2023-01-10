#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

std::string	retrieveNickname(std::string msg_to_parse);
static bool	containsInvalidCharacters(std::string nickname);
static bool	isAlreadyUsed(Server *server, std::string new_nickname);

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
	Client		client		= retrieveClient(server, client_fd);

	if (nickname.empty())
	{
		std::string ERR_NONICKNAMEGIVEN = "431 " + client.getNickname() + " :There is no nickname.\r\n";
		send(client_fd, ERR_NONICKNAMEGIVEN.c_str(), ERR_NONICKNAMEGIVEN.size(), 0);
		std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << ERR_NONICKNAMEGIVEN << RESET << std::endl;
	} 
	else if (containsInvalidCharacters(nickname))
	{
		// ERR_ERRONEUSNICKNAME
		std::string ERR_ERRONEUSNICKNAME = "432 " + client.getNickname() + " " + nickname + " :Erroneus nickname\r\n";
		send(client_fd, ERR_ERRONEUSNICKNAME.c_str(), ERR_ERRONEUSNICKNAME.size(), 0);
		std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << ERR_ERRONEUSNICKNAME << RESET << std::endl; 
	} 
	else if (isAlreadyUsed(server, nickname) == true)
	{
		// ERR_NICKNAMEINUSE
		std::string ERR_NICKNAMEINUSE = "433 " + client.getNickname() + " " + nickname + " :Nickname is already in use.\r\n";
		send(client_fd, ERR_NICKNAMEINUSE.c_str(), ERR_NICKNAMEINUSE.size(), 0);
		std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << ERR_NICKNAMEINUSE << RESET << std::endl;
	} else {
		client.setOldNickname(client.getNickname());
		std::cout << "[Server] Nickname change registered. Old nickname is now : " << client.getOldNickname() << std::endl;
		client.setNickname(nickname);
		std::string RPL_NICK = client.getOldNickname() + "!" + client.getUsername() + "@localhost :" + client.getOldNickname() + " changed their nickname to " + client.getNickname() + "\r\n";
		std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << RPL_NICK << RESET << std::endl;
	}
}

std::string	retrieveNickname(std::string msg_to_parse)
{
	std::string nickname;
	
	char *str = const_cast<char *>(msg_to_parse.data());
	nickname = strtok(str, " ");
	
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

static bool	isAlreadyUsed(Server *server, std::string new_nickname)
{
	std::map<const int, Client>	client_list;
	client_list = server->getClients();

	std::map<const int, Client>::iterator	client;
	client = client_list.begin();

	for (client; client != client_list.end(); client++)
	{
		if (client->second.getNickname() == new_nickname)
			return (true);
	}
	return (false);
}