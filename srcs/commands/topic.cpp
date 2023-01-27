#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

std::string	findChannelName(std::string msg_to_parse);
std::string	findTopic(std::string msg_to_parse);
static void	broadcastToChannel(Server *server, Channel &channel, Client &client, std::string kicked, std::string reason);

/**
 * @brief Command : TOPIC <channel> [<topic>]
 * 			
 * 	The TOPIC command is used to change or view the topic of the given channel. 
 * 	If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned 
 * 	specifying the current channel topic or lack of one. 
 * 	If <topic> is an empty string, the topic for the channel will be cleared.
 * 
 * 	Numeric Replies:
 * 	
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_NOSUCHCHANNEL (403)
 * 	ERR_NOTONCHANNEL (442)
 * 	RPL_NOTOPIC (331)
 * 	RPL_TOPIC (332)
 * 	
 * 	Examples:
 * 	[CLIENT] TOPIC #test :New topic
 *  [SERVER] ; Setting the topic on "#test" to "New topic".
 * 
 * 	[CLIENT] TOPIC #test :
 * 	[SERVER] ; Clearing the topic on "#test"
 * 
 * 	[CLIENT] TOPIC #test
 * 	[SERVER] ; Checking the topic for "#test"
 * 
 */
void	topic(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	std::string channel_name;
	std::string	topic;
	
	Client&		client = retrieveClient(server, client_fd);
	std::string client_nickname = client.getNickname();

	// ETAPE 1 - PARSER POUR TROUVER UN EVENTUEL CHANNEL_NAME
	channel_name = findChannelName(cmd_infos.message);
	if (channel_name.empty())
	{
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client_nickname, cmd_infos.name));
		return ;
	}
	
	// ETAPE 2 - RECUPERER LE CHANNEL GRACE AU CHANNEL NAME
	std::map<std::string, Channel>&			 channels = server->getChannels();
	std::map<std::string, Channel>::iterator channel = channels.find(channel_name);
	if (channel == channels.end())
	{
		addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(client_nickname, channel_name));
		return ;
	}
	if (channel->second.doesClientExist(client_nickname) == false)
	{
		addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(client_nickname, channel_name));
		return ;
	}

	topic = findTopic(cmd_infos.message);
	
	if (topic.empty()) // Afficher le topic
	{
		if (channel->second.getTopic().empty() == false)
			addToClientBuffer(server, client_fd, RPL_TOPIC(client_nickname, channel_name, channel->second.getTopic()));
		else
			addToClientBuffer(server, client_fd, RPL_NOTOPIC(client_nickname, channel_name));
	}
	else  // reattribuer le topic
	{
		if (channel->second.getMode().find('t') != std::string::npos \
			&& channel->second.isOperator(client_nickname) == false)
			addToClientBuffer(server, client_fd, ERR_CHANOPRIVSNEEDED(client_nickname, channel_name));
		else
		{
			if (topic == ":") // erase le topic
				topic.clear();
			channel->second.setTopic(topic);
			broadcastToChannel(server, channel->second, client, channel_name, topic);
		}
		
	}

}

// Possible output : | #test :New topic|
// Possible output : | test :New topic|		-> gives an error
// Possible output : | :New topic|			-> gives an error
// Possible output : | #test|
// Possible output : | #test hello|
// Possible output : | :|					-> gives an error
std::string	findChannelName(std::string msg_to_parse)
{
	std::string channel_name;
	channel_name.clear();
	
	if (msg_to_parse.empty() || msg_to_parse.find("#") == msg_to_parse.npos) // Si pas d'arg ou pas de chan (#)
		return (channel_name);
	else if (msg_to_parse.find(":") != msg_to_parse.npos) // Avec irssi, meme avec un mot, le client rajoute un ':'
	{
		char *str = const_cast<char *>(msg_to_parse.data());
		channel_name = strtok(str, " ");
		channel_name.erase(channel_name.find("#"), 1);
	}
	else
	{
		size_t i = 0;
		while (msg_to_parse[i] && (!isalpha(msg_to_parse[i]) && !isdigit(msg_to_parse[i]) && msg_to_parse[i] != '-' && msg_to_parse[i] != '_'))
			i++;
		while (msg_to_parse[i] && (isalpha(msg_to_parse[i]) || msg_to_parse[i] == '-' || msg_to_parse[i] == '_' || isdigit(msg_to_parse[i])))
			channel_name += msg_to_parse[i++];
	}
	return (channel_name);
}

std::string	findTopic(std::string msg_to_parse)
{
	std::string topic;
	

	if (msg_to_parse.empty() || msg_to_parse.find(":") == msg_to_parse.npos)
		topic.clear();
	else
	{
		topic.append(msg_to_parse, msg_to_parse.find(":"), std::string::npos);
	}
	return (topic);
}


static void	broadcastToChannel(Server *server, Channel &channel, Client &client, std::string channel_name, std::string topic)
{
	std::map<std::string, Client>::iterator member = channel.getClientList().begin();
	std::string								client_nickname = client.getNickname();
	
	while (member != channel.getClientList().end())
	{
		addToClientBuffer(server, member->second.getClientFd(), RPL_TOPIC(client_nickname, channel_name, topic));
		member++;
	}
}