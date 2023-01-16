#include "Irc.hpp"
#include "Channel.hpp"
#include "Commands.hpp"

bool			containsAtLeastOneAlphaChar(std::string str);
std::string		getChannelName(std::string msg_to_parse);
void			addChannel(Server *server, std::string const &channelName);
void			addClientToChannel(Server *server, std::string &channelName, Client &client);
void			sendChanInfos(Channel &channel, std::string channel_name, Client &client);
/**
 * @brief The JOIN command indicates that the client wants to join the given channel(s), 
 * 	each channel using the given key for it. The server receiving the command checks 
 * 	whether or not the client can join the given channel, and processes the request. 
 * 
 * 	While a client is joined to a channel, they receive all relevant information about 
 * 	that channel including the JOIN, PART, KICK, and MODE messages affecting the channel. 
 * 	They receive all PRIVMSG and NOTICE messages sent to the channel, and they also 
 * 	receive QUIT messages from other clients joined to the same channel (to let them 
 * 	know those users have left the channel and the network). 
 * 
 *  Numeric Replies:
 *  
 *  ERR_NEEDMOREPARAMS (461)
 *  ERR_NOSUCHCHANNEL (403)
 *  ERR_TOOMANYCHANNELS (405)
 *  ERR_BADCHANNELKEY (475)
 *  ERR_BANNEDFROMCHAN (474)
 *  ERR_CHANNELISFULL (471)
 *  ERR_INVITEONLYCHAN (473)
 *  ERR_BADCHANMASK (476)
 *  RPL_TOPIC (332)
 *  RPL_TOPICWHOTIME (333)
 *  RPL_NAMREPLY (353)
 *  RPL_ENDOFNAMES (366)
 *  
 *  Examples:
 * 	[CLIENT]  JOIN #foobar
 *  [SERVER] ; join channel #foobar.
 * 
 * 	[CLIENT]  JOIN #foo,#bar fubar,foobar
 * 	[SERVER]; join channel #foo using key "fubar" and channel #bar using key "foobar".
 */
void	join(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client		client 	= retrieveClient(server, client_fd);
	std::string	channel_name;

	if (containsAtLeastOneAlphaChar(cmd_infos.message) == false)
		sendServerRpl(client_fd, ERR_NEEDMOREPARAMS(client.getNickname(), cmd_infos.name));
	while (containsAtLeastOneAlphaChar(cmd_infos.message) == true)
	{
		channel_name.clear();
		channel_name = getChannelName(cmd_infos.message);

		// erase de la string le channel = "#foo,#bar" devient "#,#bar"
		cmd_infos.message.erase(cmd_infos.message.find(channel_name), channel_name.length()); 
		
		
		// Récupérer le bon channel grâce au channel name
		std::map<std::string, Channel>&			 channels 	= server->getChannels();
		std::map<std::string, Channel>::iterator it			= channels.find(channel_name);
		if (it == channels.end()) // si on ne le trouve pas, créer le channel
			addChannel(server, channel_name);
		
		// vérifier si le client est banned avant de le join au channel
		std::string client_nickname = client.getNickname();
		std::map<std::string, Channel>::iterator it_chan = server->getChannels().find(channel_name);
		if (it_chan->second.isBanned(client_nickname) == true) {
			sendServerRpl(client_fd, ERR_BANNEDFROMCHAN(client_nickname, channel_name));
			return ;
		} 
		else {
			addClientToChannel(server, channel_name, client);
			// if le channel a pas d'operateur :
			if (it_chan->second.getOperators().empty())
				it_chan->second.addFirstOperator(client.getNickname());
			
			sendChanInfos(it_chan->second, channel_name, client);
			
		}
		// TODO : prevoir check de la key; tester key et erase key du cmd_infos.msg
	}
	
}

// If a client’s JOIN command to the server is successful, the server MUST send, in this order:
// A JOIN message with the client as the message <source> and the channel they have 
// joined as the first parameter of the message.
// The channel’s topic (with RPL_TOPIC (332) and optionally RPL_TOPICWHOTIME (333)), 
// and no message if the channel does not have a topic.

void		sendChanInfos(Channel &channel, std::string channel_name, Client &client)
{
	int			client_fd	= client.getClientFd();
	std::string	nick		= client.getUsername();
	std::string username	= client.getNickname();
	std::string	client_id	= ":" + nick + "!" + username + "@localhost";
 	
	sendServerRpl(client_fd, RPL_JOIN(username, nick, channel_name));
	if (channel.getTopic().empty() == false)
	{
		sendServerRpl(client_fd, RPL_TOPIC(client_id, channel_name, channel.getTopic()));
		sendServerRpl(client_fd, RPL_DISPLAYTOPIC(client_id, channel_name));
	}
		

	// TODO: DES QUE NAMES EST FAIT / A list of users currently joined to the channel (with one or more RPL_NAMREPLY (353) 
	// numerics followed by a single RPL_ENDOFNAMES (366) numeric).
	// These RPL_NAMREPLY messages sent by the server MUST include the requesting client 
	// that has just joined the channel.

}

bool		containsAtLeastOneAlphaChar(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (isalpha(str[i]))
			return (true);
	}
	return (false);
}

std::string	getChannelName(std::string msg_to_parse)
{
	std::cout << "The msg_to_parse looks like this : |" << msg_to_parse << "|" << std::endl;
	// Expected output : | #foobar|
	// Expected output 2 : | #foo,#bar fubar,foobar|

	std::string channel_name;
	size_t i = 0;
	while (!isalpha(msg_to_parse[i]))
		i++;
	while (isalpha(msg_to_parse[i]) || msg_to_parse[i] == '-' || msg_to_parse[i] == '_') // as soon as there is a space or comma, it means the word is finished
		channel_name += msg_to_parse[i++];
	std::cout << "The channel name is : |" << channel_name << "|" << std::endl;
	return (channel_name);
}

void	addChannel(Server *server, std::string const &channelName)
{
	// check if channel already exists.
	std::map<std::string, Channel>::iterator it = server->getChannels().find(channelName);
	if (it != server->getChannels().end())
	{
		std::cout << "Channel already exists, choose an other name\n";
		return ;
	}
	Channel	channel(channelName);
	server->getChannels().insert(std::pair<std::string, Channel>(channelName, channel));
	std::cout << RED << "Channel added: " << channelName << RESET << std::endl;
}

void	addClientToChannel(Server *server, std::string &channelName, Client &client)
{
	std::map<std::string, Channel>::iterator it;
	it = server->getChannels().find(channelName);
	std::string client_nickname = client.getNickname();
	if (it->second.doesClientExist(client_nickname) == false)
	{
		it->second.getClientList().insert(std::pair<std::string, Client>(client.getNickname(), client));
		std::cout << "Client successfully joined the channel" << channelName << "!" << std::endl;
	}
	else 
		std::cout << YELLOW << client.getNickname() << "already here\n" << RESET;
}
