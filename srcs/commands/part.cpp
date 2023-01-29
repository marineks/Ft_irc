#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static bool			containsAtLeastOneAlphaChar(std::string str);
static void			broadcastToAllChannelMembers(Server *server, Channel &channel, std::string user, std::string nick, std::string reason);

/**
 * @brief The PART command removes the client from the given channel(s).
 * 
 * 	Syntax: PART <channel>{,<channel>} [<reason>]
 * 
 *  On sending a successful PART command, the user will receive a PART message 
 *  from the server for each channel they have been removed from. 
 *  <reason> is the reason that the client has left the channel(s).
 * 
 *  Numeric Replies:
 *   ERR_NEEDMOREPARAMS (461)
 *   ERR_NOSUCHCHANNEL (403)
 *   ERR_NOTONCHANNEL (442)
 * 	
 * 	Example:
 * 	[IRSSI] /PART #test,#hey :Dining
 * 	[SERVER] leaves both channels #test and #hey with the reason "Dining"
 * 	[SERVER to CLIENT]"@user_id PART #channel Dining" (for EACH channel they leave)
 */
void				part(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client		client		= retrieveClient(server, client_fd);
	std::string nick		= client.getNickname();
	std::string	channel;

	std::string reason = getReason(cmd_infos.message);
	if (cmd_infos.message.empty() == true)
	{
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(nick, cmd_infos.name));
		return ;
	}
	cmd_infos.message.erase(cmd_infos.message.find(reason), reason.length()); // #chan reason" becomes "#chan "

	while (containsAtLeastOneAlphaChar(cmd_infos.message) == true)
	{
		channel.clear();
		channel = getChannelName(cmd_infos.message);
		cmd_infos.message.erase(cmd_infos.message.find(channel), channel.length()); 

		std::map<std::string, Channel>&			 channels 	= server->getChannels();
		std::map<std::string, Channel>::iterator it			= channels.find(channel);
		if (it == channels.end()) // a) if chan does not exist
		{
			addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(nick, channel));
			continue ;
		}
		else if (it != channels.end() \
				&& it->second.doesClientExist(nick) == false) // b) if chan exists and client not in it
		{
			addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(nick, channel));
			continue ;
		}
		else // c) if successful command
		{
			it->second.getClientList().erase(nick);
			addToClientBuffer(server, client_fd, RPL_PART(user_id(nick, client.getUsername()), channel, reason));
			broadcastToAllChannelMembers(server, it->second, client.getUsername(), nick, reason);
		}
	}
}

std::string	getReason(std::string msg_to_parse)
{
	std::string reason;
	
	reason.clear(); // by default let's say there is no reason = "#hey"
	
	if (msg_to_parse.find(":") != msg_to_parse.npos) // If there is a ":", there is a reason => "#hey :bonjour"
		reason.append(msg_to_parse, msg_to_parse.find(":") + 1, std::string::npos);
	return (reason); // expected outcome : "bonjour"
}

static bool			containsAtLeastOneAlphaChar(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (isalpha(str[i]))
			return (true);
	}
	return (false);
}

static void			broadcastToAllChannelMembers(Server *server, Channel &channel, std::string user, std::string nick, std::string reason)
{
	std::map<std::string, Client>::iterator member = channel.getClientList().begin();
	
	while (member != channel.getClientList().end())
	{
		addToClientBuffer(server, member->second.getClientFd(), RPL_PART(user_id(nick, user), channel.getName(), reason));
		member++;
	}
}