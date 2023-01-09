#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static std::string	findNickname(std::string msg_to_parse);
static std::string	findChannel(std::string msg_to_parse);
/**
 * @brief The INVITE command is used to invite a user to a channel. 
 * 	The parameter <nickname> is the nickname of the person to be invited to the 
 * 	target channel <channel>.
 * 
 * 	Syntax : INVITE <nickname> <channel>
 * 
 */
void	invite(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	std::string channel = findChannel(cmd_infos.message);
	std::string nickname = findNickname(cmd_infos.message);
	
	// Check if the channel exists (if not = ERR_NOSUCHCHANNEL)

	// Check that the person inviting is a member of said channel (if not = ERR_NOTONCHANNEL)

	// Check that, if the channel is on invite-only mode, the user inviting is an operator

	// Check that the invited user is not already on the channel (if not = ERR_USERONCHANNEL)
	
	// If all checks are successful => send a RPL_INVITING to the inviting user 
	//							   and send an INVITE msg to the invited user
}

// Exemple of user input : "INVITE Wiz #foo_bar"
// 							=> msg_to_parse is " Wiz #foo_bar"
std::string	findNickname(std::string msg_to_parse)
{
	std::string nickname;
	
	char *str = const_cast<char *>(msg_to_parse.data());
	nickname = strtok(str, " ");
	
	if (nickname.empty() 							// Si pas de token après INVITE
		|| nickname.find("#") != nickname.npos)		// Si le seul token est le channel
		nickname.clear();
	return (nickname);
}

std::string	findChannel(std::string msg_to_parse)
{
	std::string channel;

	if (msg_to_parse.empty() || msg_to_parse.find("#") == msg_to_parse.npos)
	{
		channel.clear();
	}
	else
	{
		channel.append(msg_to_parse, msg_to_parse.find("#") + 1, std::string::npos);
	}
	return (channel);
}