#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static std::string	findAnyChannel(std::string msg_to_parse);
static std::string	getRplList(std::string client_nick, std::map<std::string, Channel>::iterator &channel);

/**
 * @brief If the exact name of a channel is given, the only information about 
 * 		this channel is requested; otherwise, a list of all channels will be 
 * 		displayed.
 * 
 * 	Syntax : 
 * 	[IRSSI] LIST [-yes] [<channel>]
 * 	
 * 	Numeric Replies:
 * 		RPL_LISTSTART (321) : marks the start of a channel list. 
 * 		RPL_LIST (322) : sends information about a channel to the client.
 * 		RPL_LISTEND (323) : indicates the end of a LIST response.
 * 
 * 	Examples:
 * 		/LIST
 * 		/LIST -yes => "LIST" when received by server
 * 		/LIST #ubuntu
 */
void		list(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	std::string channel_to_display	= findAnyChannel(cmd_infos.message);
	Client&		client 				= retrieveClient(server, client_fd);
	std::string client_nick 		= client.getNickname();
	std::string	RPL_LISTSTART		= "321 " + client_nick + " Channel :Users Name\r\n";
	std::string	RPL_LIST;
	std::string	RPL_LISTEND 		= "323 " + client_nick + " :End of /LIST\r\n";

	if (channel_to_display.empty()) // "/LIST" => list all channels
	{
		if (server->getChannels().empty()) {
			send(client_fd, RPL_LISTEND.c_str(), RPL_LISTEND.size(), 0);
			std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << RPL_LISTEND << RESET << std::endl;
		} 
		else 
		{
			std::map<std::string, Channel>::iterator it = server->getChannels().begin();
			while (it != server->getChannels().end())
			{
				RPL_LIST.clear();
				RPL_LIST = getRplList(client_nick, it);
				send(client_fd, RPL_LIST.c_str(), RPL_LIST.size(), 0);
				std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << RPL_LIST << RESET << std::endl;
				it++;
			}
		}
	}
	else
	{
		std::map<std::string, Channel>			 channels = server->getChannels();
		std::map<std::string, Channel>::iterator channel = channels.find(channel_to_display);
		if (channel != channels.end())
		{	
			RPL_LIST = getRplList(client_nick, channel);
			send(client_fd, RPL_LIST.c_str(), RPL_LIST.size(), 0);

		} else {
			std::cout << "[Server] The channel " << channel_to_display << " does not exist." << std::endl;
			send(client_fd, RPL_LISTEND.c_str(), RPL_LISTEND.size(), 0);
			std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << RPL_LISTEND << RESET << std::endl;
		}
	}
	return ;
}

static std::string	findAnyChannel(std::string msg_to_parse)
{
	std::string	channel;
	channel.clear();

	if (msg_to_parse.empty() || msg_to_parse.find("#") == msg_to_parse.npos) // Si pas d'arg ou pas de chan (#)
		return (channel);
	else
	{
		size_t i = 0;
		while (msg_to_parse[i] && (!isalpha(msg_to_parse[i]) && !isdigit(msg_to_parse[i]) && msg_to_parse[i] != '-' && msg_to_parse[i] != '_'))
			i++;
		while (msg_to_parse[i] && (isalpha(msg_to_parse[i]) || msg_to_parse[i] == '-' || msg_to_parse[i] == '_' || isdigit(msg_to_parse[i])))
			channel += msg_to_parse[i++];
		return (channel);
	}
}

static std::string	getRplList(std::string client_nick, std::map<std::string, Channel>::iterator &channel)
{
	std::stringstream concat;
		
	concat << "322 " << client_nick << " " << channel->second.getName() << " "  \
			<< channel->second.getClientList().size() \
			<< (channel->second.getTopic().empty() ? " :No topic set for this channel yet."  : channel->second.getTopic()) \
			<< "\r\n";
	return (concat.str());			
}
