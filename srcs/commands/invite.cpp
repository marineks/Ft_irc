#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"

std::string	findNickname(std::string msg_to_parse);

/**
 * @brief The INVITE command is used to invite a user to a channel. 
 * 	The parameter <nickname> is the nickname of the person to be invited to the 
 * 	target channel <channel>.
 * 
 * 	Syntax : INVITE <nickname> <channel>
 * 
 */
void	invite(Server server, int const client_fd, cmd_struct cmd_infos)
{
	// Parsing
	std::string nickname = findNickname(cmd_infos.message);
	std::string channel = findChannel(cmd_infos.message);

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
	// split sur les espaces

	// check que le 1er token n'a pas de "#"

	// le renvoyer
}