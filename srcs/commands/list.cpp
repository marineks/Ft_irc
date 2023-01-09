#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static std::string	findAnyChannel(std::string msg_to_parse);

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
	Client		client 				= retrieveClient(server, client_fd);
	std::string client_nick 		= client.getNickname();
	std::string	RPL_LISTSTART		= "321 " + client_nick + " Channel :Users Name\r\n";
	std::string	RPL_LIST;
	std::string	RPL_LISTEND 		= "323 " + client_nick + " :End of /LIST\r\n";

	if (channel_to_display.empty()) // "/LIST" => list all channels
	{
		if (server->getChannels().empty()) {
			send(client_fd, RPL_LISTEND.c_str(), RPL_LISTEND.size(), 0);
			std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << RPL_LISTEND << RESET << std::endl;
		} else {
			std::stringstream concat;
			for (std::map<std::string, Channel>::iterator it = server->getChannels().begin(); \
					it != server->getChannels().end(); \
					it++)
			{
				concat.str(""); // clear le stream
				RPL_LIST.clear();

				concat << "322 " << client_nick << " " << it->second.getName() << " "  \
						<< it->second.getClientList().size() \
						<< (it->second.getTopic().empty() ? " :No topic set for this channel yet."  : it->second.getTopic()) \
						<< "\r\n";				
				RPL_LIST = concat.str();
				send(client_fd, RPL_LIST.c_str(), RPL_LIST.size(), 0);
				std::cout << "[Server] Message sent to client " << client_fd << " >> " << CYAN << RPL_LIST << RESET << std::endl;
			}

		}
		
	}
	// else
	// {
	// 	// check if channel found exists

	// 	// if it exists, display
	// 	// else, print END of LIST
	// }
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
		while (!isalpha(msg_to_parse[i]))
			i++;
		while (isalpha(msg_to_parse[i]))
			channel += msg_to_parse[i++];
		return (channel);
	}
}
