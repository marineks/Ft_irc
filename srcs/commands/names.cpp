#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static bool			containsAtLeastOneAlphaChar(std::string str);
static std::string	getaChannelName(std::string msg_to_parse);

/**
 * @brief The NAMES command is used to view the nicknames joined to a channel.
 *  If the channel name is invalid or the channel does not exist, one RPL_ENDOFNAMES 
 * 	numeric containing the given channel name should be returned.
 * 
 * 	Syntax: NAMES <channel>{,<channel>}
 * 	
 * 	Numeric Replies:
 * 	
 * 	RPL_NAMREPLY (353)
 * 	RPL_ENDOFNAMES (366)
 * 
 * 	Examples:
 * 	[CLIENT] /NAMES #test,#42
 * 	[SERVER] <client> <symbol> #test :<nick1> <nick2>
 * 	
 */
void	names(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client			client				= retrieveClient(server, client_fd);
	std::string		symbol				= "=";
	std::string		channel_to_name;
	std::string		list_of_members;

	if (cmd_infos.message.empty() == true)
	{
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client.getNickname(), cmd_infos.name));
		return ;
	}
	while (containsAtLeastOneAlphaChar(cmd_infos.message) == true)
	{
		// find the channel to display names of
		channel_to_name.clear();
		channel_to_name = getaChannelName(cmd_infos.message);
		cmd_infos.message.erase(cmd_infos.message.find(channel_to_name), channel_to_name.length()); 

		// Error handling (Inexistent channel, Secret Mode on...)
		std::map<std::string, Channel>				channels = server->getChannels();
		std::map<std::string, Channel>::iterator	channel = channels.find(channel_to_name);
		if (channel == channels.end()\
			|| (channel->second.doesClientExist(client.getNickname()) == false \
			&& channel->second.getMode().find('s') != std::string::npos))
		{
			addToClientBuffer(server, client_fd, RPL_ENDOFNAMES(client.getNickname(), channel_to_name));
			continue ;
		}
			
		// find the symbol of said channel (public, secret, or private)
		symbol.clear();
		symbol = getSymbol(channel->second);

		// get as a string the list of all members (by nickname)
		list_of_members.clear();
		list_of_members = getListOfMembers(client.getNickname(), channel->second);

		if (list_of_members.empty() == false)
			addToClientBuffer(server, client_fd, RPL_NAMREPLY(client.getNickname(), symbol, channel_to_name, list_of_members));
		addToClientBuffer(server, client_fd, RPL_ENDOFNAMES(client.getNickname(), channel_to_name));
	}
	
}

static bool		containsAtLeastOneAlphaChar(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (isalpha(str[i]) || str[i] == '_' || str[i] == '-' || isdigit(str[i]))
			return (true);
	}
	return (false);
}

static std::string getaChannelName(std::string msg_to_parse)
{
	std::string	channel_name;
	size_t		i = 0;

	while (msg_to_parse[i] && (!isalpha(msg_to_parse[i]) && !isdigit(msg_to_parse[i]) && msg_to_parse[i] != '-' && msg_to_parse[i] != '_'))
		i++;
	while (msg_to_parse[i] && (isalpha(msg_to_parse[i]) || msg_to_parse[i] == '-' || msg_to_parse[i] == '_' || isdigit(msg_to_parse[i])))
	{
		channel_name += msg_to_parse[i];
		i++;
	}
	return (channel_name);
}
