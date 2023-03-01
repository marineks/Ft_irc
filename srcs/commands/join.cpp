#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

bool			containsAtLeastOneAlphaChar(std::string str);
std::string		retrieveKey(std::string msg_to_parse);
void			addChannel(Server *server, std::string const &channelName);
void			addClientToChannel(Server *server, std::string &channelName, Client &client);
void			sendChanInfos(Server *server, Channel &channel, std::string channel_name, Client &client);
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
	Client		client 			= retrieveClient(server, client_fd);
	std::string client_nickname = client.getNickname();
	std::string	channel_name;

	if (containsAtLeastOneAlphaChar(cmd_infos.message) == false)
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client_nickname, cmd_infos.name));
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
		{
			addChannel(server, channel_name);	
		}
		else if (it->second.getMode().find('k') != std::string::npos) // Si channel en mode +k
		{
			std::string key = retrieveKey(cmd_infos.message);
			cmd_infos.message.erase(cmd_infos.message.find(key), key.length()); // on erase la key de la string
			if (key != it->second.getChannelPassword())
			{
				addToClientBuffer(server, client_fd, ERR_BADCHANNELKEY(client_nickname, channel_name));
				continue; // on passe la suite, au prochain channel à ajouter síl y en a un
			}
		}

		std::map<std::string, Channel>::iterator it_chan = server->getChannels().find(channel_name);
		// vérifier si le channel est full
		if (it_chan->second.getCapacityLimit() != -1 && (int)it_chan->second.getClientList().size() == it_chan->second.getCapacityLimit())
		{
			addToClientBuffer(server, client_fd, ERR_CHANNELISFULL(client_nickname, channel_name));
			continue ; // on passe la suite, au prochain channel à ajouter síl y en a un
		}
		// vérifier si le client est banned avant de le join au channel
		if (it_chan->second.getMode().find("b") != std::string::npos && it_chan->second.isBanned(client_nickname) == true)
		{
			addToClientBuffer(server, client_fd, ERR_BANNEDFROMCHAN(client_nickname, channel_name));
		} 
		else 
		{
			addClientToChannel(server, channel_name, client);
			if (it_chan->second.getOperators().empty())
				it_chan->second.addFirstOperator(client.getNickname());
			sendChanInfos(server, it_chan->second, channel_name, client);
		}
	}
	
}

/**
 * @brief If a client’s JOIN command to the server is successful, the server MUST send, in this order:
 * 
 * 	1) A JOIN message
 * 	2) The channel's TOPIC if there is one (RPL_TOPIC)
 * 	3) The NAMES of the users in this channel
 *  
 * [:msanjuan_!msanjuan@localhost 353 msanjuan_ = #hello :@msanjuan_ ]
 */
void		sendChanInfos(Server *server, Channel &channel, std::string channel_name, Client &client)
{
	std::string	nick		= client.getNickname();
	std::string username	= client.getUsername();
 	
	std::map<std::string, Client>::iterator member = channel.getClientList().begin();

	while (member != channel.getClientList().end())
	{
		addToClientBuffer(server, member->second.getClientFd(), RPL_JOIN(user_id(nick, username), channel_name));
		if (channel.getTopic().empty() == false)
			addToClientBuffer(server, member->second.getClientFd(), RPL_TOPIC(nick, channel_name, channel.getTopic()));
		
		std::string	list_of_members = getListOfMembers(nick, channel);
		std::string symbol			= getSymbol(channel);

		addToClientBuffer(server, member->second.getClientFd(), RPL_NAMREPLY(username, symbol, channel_name, list_of_members));
		addToClientBuffer(server, member->second.getClientFd(), RPL_ENDOFNAMES(username, channel_name));
		member++;
	}
}

bool		containsAtLeastOneAlphaChar(std::string str)
{
	// If +k mode activated, the input is " #foo,#bar fubar,foobar".
	// But we only want this part : "#foo,#bar"
	
	if (str[0] == ' ')
		str.erase(0, 1);
	if (str.find(" ") != str.npos)
		str = str.substr(0, str.find(" "));
	for (size_t i = 0; i < str.size(); i++)
	{
		if (isalpha(str[i]))
			return (true);
	}
	return (false);
}

std::string	retrieveKey(std::string msg_to_parse)
{
	std::string	key;
	key.clear();
	
	msg_to_parse = msg_to_parse.substr(msg_to_parse.find_last_of(" "), msg_to_parse.npos);
	if (msg_to_parse[0] == ' ')
		msg_to_parse.erase(0, 1); // Expected output : |fubar_75,foobar|
		
	int	begin_pos = (msg_to_parse.find(",") == 0) ? msg_to_parse.find(",") + 1 : 0; // Expected: begin à |fubar_75,foobar| ou |foobar| si 2eme key
	
	while ( msg_to_parse[begin_pos] != ',' && msg_to_parse[begin_pos])
	{
		if (msg_to_parse[begin_pos] == '_' || msg_to_parse[begin_pos] == '-'|| isalpha(msg_to_parse[begin_pos]) || isdigit(msg_to_parse[begin_pos]))
			key += msg_to_parse[begin_pos];
		begin_pos++;
	}
	return (key);
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
}

void	addClientToChannel(Server *server, std::string &channelName, Client &client)
{
	std::map<std::string, Channel>::iterator it;
	it = server->getChannels().find(channelName);
	std::string client_nickname = client.getNickname();
	if (it->second.doesClientExist(client_nickname) == false)
	{
		it->second.getClientList().insert(std::pair<std::string, Client>(client.getNickname(), client));
	}
	else 
		std::cout << YELLOW << client.getNickname() << "already here\n" << RESET;
}
