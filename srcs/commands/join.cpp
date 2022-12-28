#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void	addChannel(Server server, Client &client,cmd_struct cmd_infos);
void	addClientToChannel(std::string &channelName, Client &client);
void	printChannel(std::string &channelName);

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
void	join(Server server, Client &client, cmd_struct cmd_infos)
{
	// TODO: coder le parsing du cmd.message pour arriver à channelName
	// TODO: ATTENTION parsing, plus complique, on peut rajouter plrs chans en meme tps
	std::string channelName;

	std::map<std::string, Channel>			 channels = server.getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end())
		addChannel(server, channelName);
	if (it->second.isBanned(client.getNickname()) == SUCCESS) // BUG : comprends pas pq ca compile pas
	{
		std::cout << client.getNickname() << " is banned from " << channelName << std::endl; 
		return ;
	}
	addClientToChannel(server, channelName, client);
	it->second.addFirstOperator(client.getNickname());

	// TODO: Attention, cest pas fini quand qqun est add au chan, il y a plein d'infos à lui fournir
}


void	addChannel(Server server, std::string const &channelName)
{
	// check if channel already exists.
	std::map<std::string, Channel>			 channels = server.getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::cout << "Channel already exists, choose an other name\n";
		return ;
	}
	Channel	channel(channelName);
	channels.insert(std::pair<std::string, Channel>(channelName, channel));
	// std::cout << RED << "Channel added: " << channelName << RESET << std::endl;
}

void	addClientToChannel(Server server, std::string &channelName, Client &client)
{
	std::map<std::string, Channel>			 channels = server.getChannels();
	std::map<std::string, Channel>::iterator it;
	it = channels.find(channelName);
	if (it->second.doesClientExist(client.getNickname()) == false) // BUG : comprends pas pq ca compile pas
	{
		it->second.addClientToChannel(client);
	}
	else 
		std::cout << YELLOW << client.getNickname() << "already here\n" << RESET;
}

// NOTE: à qui sert cette fonction ? debug ?
void	printChannel(Server server, std::string &channelName)
{
	std::map<std::string, Channel>			 	channels = server.getChannels();
	std::map<std::string, Channel> 				tmp; // NOTE: pourquoi un tmp ?
	std::map<std::string, Channel>::iterator	it = channels.find(channelName);
	it->second.printClientList();
}
