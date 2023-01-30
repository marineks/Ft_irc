#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static std::string	getKickedName(std::string msg_to_parse);
static void			broadcastToChannel(Server *server, Channel &channel, Client &client, std::string kicked, std::string reason);

/**
 * @brief The KICK command can be used to request the forced removal of a user 
 *  from a channel. It causes the <user> to be removed from the <channel> by force. 
 *  If no comment is given, the server SHOULD use a default message instead.
 * 
 *  Parameters: <channel> <user> *( "," <user> ) [<comment>]
 * 
 * Numeric Replies:
 * 
 * ERR_NEEDMOREPARAMS (461)
 * ERR_NOSUCHCHANNEL (403)
 * ERR_CHANOPRIVSNEEDED (482)
 * ERR_USERNOTINCHANNEL (441)
 * ERR_NOTONCHANNEL (442)
 * 
 * Example:
 * Client's request : KICK #Finnish John :Speaking English
 * 
 * Server's Response: " Command to kick John from #Finnish using "Speaking English" 
 * 						as the reason (comment)."
 *
 * @param server
 * @param cmd_infos Structure w/ prefix, command name and message
 */
void				kick(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client& 	requester		= retrieveClient(server, client_fd);
	std::string	requester_name	= requester.getNickname();
	std::string	channel_name	= getChannelName(cmd_infos.message);
	std::string	kicked_name		= getKickedName(cmd_infos.message);
	std::string	reason			= getReason(cmd_infos.message);

	std::map<std::string, Channel>&			 channels 	= server->getChannels();
	std::map<std::string, Channel>::iterator it_chan	= channels.find(channel_name);

	reason = (reason.empty()) ? ":Kicked by the channel's operator" : reason;

	// DEBUG
	// std::cout << "Requester : |" << requester_name << "|" << std::endl;
	// std::cout << "Reason : |" << reason << "|" << std::endl;
	// std::cout << "Kicked : |" << kicked_name << "|" << std::endl;

	if (channel_name.empty() || kicked_name.empty())
	{
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(requester_name, cmd_infos.name));
		// sendServerRpl(client_fd, ERR_NEEDMOREPARAMS(requester_name, cmd_infos.name));
		return ;
	}
	else if (it_chan == channels.end())
	{
		addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(requester_name, channel_name));
		// sendServerRpl(client_fd, ERR_NOSUCHCHANNEL(requester_name, channel_name));
		return ;
	}
	else if (it_chan->second.doesClientExist(requester_name) == false)
	{
		addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(requester_name, channel_name));
		// sendServerRpl(client_fd, ERR_NOTONCHANNEL(requester_name, channel_name));
		return ;
	}
	else if (it_chan->second.doesClientExist(kicked_name) == false)
	{
		addToClientBuffer(server, client_fd, ERR_USERNOTINCHANNEL(requester_name, kicked_name, channel_name));
		// sendServerRpl(client_fd, ERR_USERNOTINCHANNEL(requester_name, kicked_name, channel_name));
		return ;
	}
	else if (it_chan->second.isOperator(requester_name) == false) // you're not a channel operator
	{
		addToClientBuffer(server, client_fd, ERR_CHANOPRIVSNEEDED(requester_name, channel_name));
		// sendServerRpl(client_fd, ERR_CHANOPRIVSNEEDED(requester_name, channel_name));
		return ;
	}
	else
	{
		broadcastToChannel(server, it_chan->second, requester, kicked_name, reason);
		it_chan->second.getClientList().erase(kicked_name);
		it_chan->second.addToKicked(kicked_name);
	}
}

static std::string	getKickedName(std::string msg_to_parse)
{
	std::string kicked;

	if (msg_to_parse[0] == ' ')
		msg_to_parse.erase(0, 1);
	kicked = msg_to_parse.substr(msg_to_parse.find(' ') + 1, (msg_to_parse.find(':') - 1 - (msg_to_parse.find(' ') + 1)));
	return (kicked);

}

static void			broadcastToChannel(Server *server, Channel &channel, Client &client, std::string kicked, std::string reason)
{
	std::map<std::string, Client>::iterator member = channel.getClientList().begin();
	
	while (member != channel.getClientList().end())
	{
		addToClientBuffer(server, member->second.getClientFd(), \
			RPL_KICK(user_id(client.getNickname(), client.getUsername()), channel.getName(), kicked, reason));
		member++;
	}
}