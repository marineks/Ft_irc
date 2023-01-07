#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

Client		retrieveClient(Server *server, int const client_fd);
std::string	findChannelName(std::string msg_to_parse);

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
	
	Client		client = retrieveClient(server, client_fd);
	std::string client_nickname = client.getNickname();

	// ETAPE 1 - PARSER POUR TROUVER UN EVENTUEL CHANNEL_NAME
	channel_name = findChannelName(cmd_infos.message);
	if (channel_name.empty())
	{
		std::cout << "Pas de channel, pas de topic" << std::endl;
		std::string ERR_NEEDMOREPARAMS = " 461 " + client_nickname + " " + cmd_infos.name + " :Not enough parameters.\r\n";
		send(client_fd, ERR_NEEDMOREPARAMS.c_str(), ERR_NEEDMOREPARAMS.size(), 0);
		return ;
	}
	
	// ETAPE 2 - RECUPERER LE CHANNEL GRACE AU CHANNEL NAME
	std::map<std::string, Channel>			 channels = server->getChannels();
	std::map<std::string, Channel>::iterator channel = channels.find(channel_name);
	// // [!] Si channel n'existe pas, renvoyer une erreur
	if (channel == channels.end())
	{
		std::cout << "This channel does not exist." << std::endl;
		std::string ERR_NOSUCHCHANNEL = " 403 " + client_nickname + " " + channel->first + " :No such channel.\r\n";
		send(client_fd, ERR_NOSUCHCHANNEL.c_str(), ERR_NOSUCHCHANNEL.size(), 0);
		return ;
	}
	// // [!] Si user pas dans ce chan, renvoyer une erreur
	if (channel->second.doesClientExist(client_nickname) == false)
	{
		std::cout << "You are not on the channel you want to see to the topic of." << std::endl;
		std::string ERR_NOTONCHANNEL = " 442 " + client_nickname + " " + channel->first + " :The user is not on this channel.\r\n";
		send(client_fd, ERR_NOTONCHANNEL.c_str(), ERR_NOTONCHANNEL.size(), 0);
		return ;
	}

	// // Gérer le topic
	// channel->second.setTopic(findTopic(cmd_infos.message));
	// if (topic.empty())
	// {
	// 	// afficher le topic
	// }
	// else if (topic == ":")
	// {
	// 	// erase le topic
	// }
	// else
	// {
	// 	// reattribuer le topic
	// }
}

Client	retrieveClient(Server *server, int const client_fd)
{
	std::map<const int, Client>	client_list = server->getClients();
	std::map<const int, Client>::iterator it_client = client_list.find(client_fd);
	Client client = it_client->second;
	return (client);
}

std::string	findChannelName(std::string msg_to_parse)
{
	std::cout << "The msg_to_parse looks like this : |" << msg_to_parse << "|" << std::endl;
	// Possible output : | #test :New topic|
	// Possible output : | test :New topic|		-> gives an error
	// Possible output : | :New topic|			-> gives an error
	// Possible output : | #test|
	// Possible output : | #test hello|
	// Possible output : | :|					-> gives an error
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
		while (!isalpha(msg_to_parse[i]))
			i++;
		while (isalpha(msg_to_parse[i]))
			channel_name += msg_to_parse[i++];
	}
	// DEBUG
	std::cout << "[v] Channel name in TOPIC is : |" << channel_name << "|" << std::endl;
	return (channel_name);
}